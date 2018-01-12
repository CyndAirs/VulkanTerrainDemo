#include "Renderer.h"

Renderer::Renderer()
{
	vertices = {
		{ { -0.5f, -0.5f, 0.0f },{ 1.0f, 0.0f, 0.0f },{-0.5f, -0.5f,}},
		{ { 0.5f, -0.5f, 0.0f },{ 0.0f, 1.0f, 0.0f },{ 0.5f, -0.5f, } },
		{ { 0.5f, 0.5f, 0.0f },{ 0.0f, 0.0f, 1.0f },{ 0.5f, 0.5f, } },
		{ { -0.5f, 0.5f, 0.0f },{ 1.0f, 1.0f, 1.0f },{ -0.5f, 0.5f, } },
	};

	indices = {
		0, 1, 2, 2, 3, 0
	};
}

Renderer::Renderer(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::string textureSource)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textureSource = textureSource;
}

Renderer::~Renderer()
{
	delete commandManager;
	closeWindow();
}

void Renderer::run(HWND gui) {
	initWindow();
	initVulkan();
	mainLoop(gui);
}

void Renderer::initWindow()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Vulkan Terrain Demo", nullptr, nullptr);

	glfwSetWindowUserPointer(window, this);
	glfwSetWindowSizeCallback(window, Renderer::onWindowResized);
}

void Renderer::closeWindow()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Renderer::initVulkan()
{
	setupGlfw();
	#if BUILD_ENABLE_VULKAN_DEBUG
	setupDebug();
	#endif

	initInstance();

	#if BUILD_ENABLE_VULKAN_DEBUG
	initDebug();
	#endif	

	initSurface();

	deviceManager.initDevice(instance, surface);

	RendererInfo info;
	info.device = deviceManager.device;
	info.graphicsQueue = &(deviceManager.getGraphicsQueue());
	info.indices = indices;
	info.instance = instance;
	info.presentQueue = &(deviceManager.getGraphicsQueue());
	info.queueFamilyIndices = deviceManager.getQueueFamilyIndices();
	info.surface = surface;
	info.vertices = vertices;
	info.gpu = deviceManager.getPhysicalDevice();

	commandManager->init(info);

	swapChain.init(info, commandManager);

	bufferManager.init(info, &swapChain, commandManager, textureSource);

	initSemaphores();
}

void Renderer::mainLoop(HWND gui)
{
	static std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
	std::chrono::time_point<std::chrono::system_clock> loopStart = std::chrono::system_clock::now();
	std::chrono::time_point<std::chrono::system_clock> stop = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsedSeconds;
	int frameCount = 0;
	float rotationX = 0.0f, rotationY = 0.0f;
	float time;
	double fps;
	std::string label;
	std::wstring labeltemp;

	label = "Vertices: " + std::to_string(vertices.size());
	labeltemp = std::wstring(label.begin(), label.end());
	SetDlgItemText(gui, VERTICES_LABEL, (LPCWSTR)labeltemp.c_str());

	label = "Indices: " + std::to_string(indices.size());
	labeltemp = std::wstring(label.begin(), label.end());
	SetDlgItemText(gui, INDICES_LABEL, (LPCWSTR)labeltemp.c_str());

	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, Renderer::keyCallback);
	glfwSetCursorPosCallback(window, Renderer::cursorPosCallback);
	glfwSetMouseButtonCallback(window, Renderer::mouseClickCallback);

	start = std::chrono::system_clock::now();
	loopStart = std::chrono::system_clock::now();
	while (!glfwWindowShouldClose(window)) {
		
		
		glfwPollEvents();

		stop = std::chrono::system_clock::now();
		time = std::chrono::duration_cast<std::chrono::milliseconds>(stop - loopStart).count() / 1000.0f;
		if (xpos != lastXpos)
		{
			rotationX += (xpos - lastXpos) * glm::radians(1.0f);
			lastXpos = xpos;
		}
		if (ypos != lastYpos)
		{
			rotationY += (ypos - lastYpos) * glm::radians(1.0f);
			lastYpos = ypos;
		}
		if (rotateLeft) rotationX += time * glm::radians(100.0f);
		if (rotateRight) rotationX -= time * glm::radians(100.0f);
		loopStart = std::chrono::system_clock::now();

		bufferManager.updateUniformBuffer(generateUniformData(rotationX, rotationY));
		drawFrame();

		stop = std::chrono::system_clock::now();
		elapsedSeconds = stop - start;

		frameCount++;

		if (elapsedSeconds.count() > 1.0) {
			double fps = frameCount / elapsedSeconds.count();

			frameCount = 0;

			label = "FPS: " + std::to_string(fps);

			labeltemp = std::wstring(label.begin(), label.end());

			SetDlgItemText(gui, FPS_LABEL, (LPCWSTR)labeltemp.c_str());
			
			start = std::chrono::system_clock::now();
		}
		
	}

	vkDeviceWaitIdle(deviceManager.getDevice());
}

void Renderer::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Renderer* app = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));


	if ((key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) && action == GLFW_RELEASE)
		if (app->getRotateRight() == true) 
			app->stopRotation(); 

	if ((key == GLFW_KEY_A || key == GLFW_KEY_LEFT) && action == GLFW_RELEASE)
		if (app->getRotateLeft() == true)
			app->stopRotation(); 

	if ((key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) && action == GLFW_PRESS)
		if (app->getRotateLeft() == false)
			app->setRotateRight(); 

	if ((key == GLFW_KEY_A || key == GLFW_KEY_LEFT) && action == GLFW_PRESS)
		if (app->getRotateRight() == false)
			app->setRotateLeft(); 

}



void Renderer::cursorPosCallback(GLFWwindow * window, double xpos, double ypos)
{
	Renderer* app = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
	if (app->mousePressed)
	{
		app->xpos = xpos;
		app->ypos = ypos;
	}
}

void Renderer::mouseClickCallback(GLFWwindow * window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		Renderer* app = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
		app->mousePressed = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		Renderer* app = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
		app->mousePressed = false;
	}
}

bool Renderer::getRotateLeft() {
	return rotateLeft;
}

bool Renderer::getRotateRight() {
	return rotateRight;
}

void Renderer::setRotateLeft() {
	if (rotateRight == false) rotateLeft = true;
}

void Renderer::setRotateRight() {
	if (rotateLeft == false) rotateRight = true;
}

void Renderer::stopRotation() {
	rotateLeft = rotateRight = false;
}

UniformBufferObject Renderer::generateUniformData(float rotationX, float rotationY)
{

	UniformBufferObject ubo = {};
	ubo.model *= glm::rotate(glm::mat4(), rotationX, glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.model *= glm::rotate(glm::mat4(), rotationY, glm::vec3(1.0f, 0.0f, 0.0f));
	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(45.0f), swapChain.swapChainExtent.width / (float)swapChain.swapChainExtent.height, 0.1f, 10.0f);
	ubo.lightPosition = glm::vec3(-30.0f, 30.0f, 30.0f);

	ubo.proj[1][1] *= -1;


	return ubo;
}



void Renderer::onWindowResized(GLFWwindow* window, int width, int height) {
	if (width == 0 || height == 0) return;
	Renderer* app = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
	app->swapChain.recreateSwapChain();
	app->bufferManager.initCommandBuffers();
}

VkSurfaceKHR Renderer::getSurface()
{
	return surface;
}

void Renderer::initInstance()
{
	VkApplicationInfo applicationInfo = {};
	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.apiVersion = VK_API_VERSION_1_0;
	applicationInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
	applicationInfo.pApplicationName = "Terrain Demo";

	VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pApplicationInfo = &applicationInfo;
	instanceCreateInfo.enabledLayerCount = instanceLayers.size();
	instanceCreateInfo.ppEnabledLayerNames = instanceLayers.data();
	instanceCreateInfo.enabledExtensionCount = instanceExtensions.size();
	instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();
	instanceCreateInfo.pNext = &debugReportCallbackCreateInfo;

	ErrorCheck(vkCreateInstance(&instanceCreateInfo, nullptr, instance.replace()));
}

void Renderer::setupGlfw() {

	unsigned int glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	for (unsigned int i = 0; i < glfwExtensionCount; i++) {
		instanceExtensions.push_back(glfwExtensions[i]);
	}
}

void Renderer::initSurface() {
	ErrorCheck(glfwCreateWindowSurface(instance, window, nullptr, surface.replace()));
}

void Renderer::drawFrame()
{
	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(deviceManager.getDevice(), swapChain.swapChain, std::numeric_limits<uint64_t>::max(), imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		swapChain.recreateSwapChain();
		bufferManager.initCommandBuffers();
		return;
	}
	else ErrorCheck(result);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { imageAvailableSemaphore };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = bufferManager.getCommandBuffer(imageIndex);

	VkSemaphore signalSemaphores[] = { renderFinishedSemaphore };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	ErrorCheck(vkQueueSubmit(deviceManager.getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE));

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { swapChain.swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;

	result = vkQueuePresentKHR(deviceManager.getPresentQueue(), &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
		swapChain.recreateSwapChain();
		bufferManager.initCommandBuffers();
	}
	else ErrorCheck(result);
}



void Renderer::initSemaphores() {
	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	if (vkCreateSemaphore(deviceManager.getDevice(), &semaphoreInfo, nullptr, imageAvailableSemaphore.replace()) != VK_SUCCESS ||
		vkCreateSemaphore(deviceManager.getDevice(), &semaphoreInfo, nullptr, renderFinishedSemaphore.replace()) != VK_SUCCESS) {

		throw std::runtime_error("Failed to create semaphores!");
	}
}

#if BUILD_ENABLE_VULKAN_DEBUG
std::wstring get_utf16(const std::string &str, int codepage)
{
	if (str.empty()) return std::wstring();
	int sz = MultiByteToWideChar(codepage, 0, &str[0], (int)str.size(), 0, 0);
	std::wstring res(sz, 0);
	MultiByteToWideChar(codepage, 0, &str[0], (int)str.size(), &res[0], sz);
	return res;
}
/** Initilises Vulkan's validation layer */
VKAPI_ATTR VkBool32 VKAPI_CALL
VulkanDebugCallback(
	VkDebugReportFlagsEXT flags,
	VkDebugReportObjectTypeEXT objType,
	uint64_t source,
	size_t location,
	int32_t msgCode,
	const char * layerPrefix,
	const char * msg,
	void * userData
) 
{
	std::ostringstream stream;
	//if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) { stream << "[Info]" << msg << std::endl; }
	if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) { stream << "[Warning]" << msg << std::endl; }
	if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) { stream << "[Performance Warning]" << msg << std::endl; }
	if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) { stream << "[Error]" << msg << std::endl; }
	//if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) { stream << "[Debug]" << msg << std::endl; }
	//stream << msg << std::endl;
	std::cout << stream.str();

#ifdef _WIN32
	if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) { MessageBox(NULL, (LPCWSTR)get_utf16(stream.str(), CP_UTF8).c_str(), L"ERROR", 0); };
#endif

	return false;
}

void Renderer::setupDebug()
{
	debugReportCallbackCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
	debugReportCallbackCreateInfo.pfnCallback = VulkanDebugCallback;
	debugReportCallbackCreateInfo.flags =
		VK_DEBUG_REPORT_INFORMATION_BIT_EXT |
		VK_DEBUG_REPORT_WARNING_BIT_EXT |
		VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
		VK_DEBUG_REPORT_ERROR_BIT_EXT |
		VK_DEBUG_REPORT_DEBUG_BIT_EXT |
		0;
	
	instanceLayers.push_back("VK_LAYER_LUNARG_standard_validation");

	instanceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);


}

/** Creates report callback for debugging */
VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback) {
	auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pCallback);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

/** Destroys report callback for debugging */
void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
	if (func != nullptr) {
		func(instance, callback, pAllocator);
	}
}

void Renderer::initDebug()
{
	ErrorCheck(CreateDebugReportCallbackEXT(instance, &debugReportCallbackCreateInfo, nullptr, debugReportCallback.replace()));
}

#endif // BUILD_ENABLE_VULKAN_DEBUG