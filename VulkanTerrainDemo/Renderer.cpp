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

	camera = Camera(glm::vec3(0.41f, -0.07f, 2.5f), glm::vec3(0, 1, 0), 270, 360);
	firstMouse = true;
}

Renderer::Renderer(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::string textureSource)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textureSource = textureSource;
	camera = Camera(glm::vec3(0.0f, 0.0f, 2.5f), glm::vec3(0, 1, 0), 270, 0);

	firstMouse = true;
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
	std::string stateText;
	std::wstring stateTextTemp;

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

	camera.SetProjectionMatrix(glm::perspective(glm::radians(60.0f), swapChain.swapChainExtent.width / (float)swapChain.swapChainExtent.height, 0.1f, 10.0f));

	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();

		stop = std::chrono::system_clock::now();
		time = std::chrono::duration_cast<std::chrono::milliseconds>(stop - loopStart).count() / 1000.0f;
		loopStart = std::chrono::system_clock::now();

		bufferManager.updateUniformBuffer(generateUniformData(rotationX, rotationY));
		drawFrame();

		stop = std::chrono::system_clock::now();
		elapsedSeconds = stop - start;
		camera.ProcessKeyboard(forward, right, up, elapsedSeconds.count() / 1000.f);
		frameCount++;

		if (elapsedSeconds.count() > 1.0) {
			double fps = frameCount / elapsedSeconds.count();

			frameCount = 0;

			label = "FPS: " + std::to_string(fps);

			labeltemp = std::wstring(label.begin(), label.end());

			SetDlgItemText(gui, FPS_LABEL, (LPCWSTR)labeltemp.c_str());

			start = std::chrono::system_clock::now();

			stateText = "State: " + std::string(state == FLOATING ? "Flying" : "Editing");

			stateTextTemp = std::wstring(stateText.begin(), stateText.end());

			SetDlgItemText(gui, INFO_LABEL, (LPCWSTR)stateTextTemp.c_str());
			SetDlgItemText(gui, VERTICES_LABEL, (LPCWSTR)testTemp.c_str());
		}
	}
	vkDeviceWaitIdle(deviceManager.getDevice());
}

void Renderer::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Renderer* app = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
	if (app->state == FLOATING)
	{
		if ((key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) && action == GLFW_RELEASE)
			app->right = 0;
		if ((key == GLFW_KEY_A || key == GLFW_KEY_LEFT) && action == GLFW_RELEASE)
			app->right = 0;
		if ((key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) && action == GLFW_PRESS)
			app->right = 1;
		if ((key == GLFW_KEY_A || key == GLFW_KEY_LEFT) && action == GLFW_PRESS)
			app->right = -1;
		if ((key == GLFW_KEY_W || key == GLFW_KEY_UP) && action == GLFW_RELEASE)
			app->forward = 0;
		if ((key == GLFW_KEY_S || key == GLFW_KEY_DOWN) && action == GLFW_RELEASE)
			app->forward = 0;
		if ((key == GLFW_KEY_W || key == GLFW_KEY_UP) && action == GLFW_PRESS)
			app->forward = 1;
		if ((key == GLFW_KEY_S || key == GLFW_KEY_DOWN) && action == GLFW_PRESS)
			app->forward = -1;
		if (key == GLFW_KEY_E && action == GLFW_PRESS)
			app->up = -1;
		if (key == GLFW_KEY_E && action == GLFW_RELEASE)
			app->up = 0;
		if (key == GLFW_KEY_Q && action == GLFW_PRESS)
			app->up = 1;
		if (key == GLFW_KEY_Q && action == GLFW_RELEASE)
			app->up = 0;
	}
	if (key == GLFW_KEY_PAGE_UP && action == GLFW_PRESS)
		if(app->editStrength < 1.0f)
			app->editStrength += 0.01f;
	if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_PRESS)
		if (app->editStrength > 0.01f)
			app->editStrength -= 0.01f;
	if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS)
		if(app->editingSize < 0.1f)
			app->editingSize += 0.01f;
	if (key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS)
		if (app->editingSize > 0.01f)
			app->editingSize -= 0.01f;

	if (key == GLFW_KEY_F && action == GLFW_RELEASE)
	{
		if (app->state == FLOATING)
		{
			app->state = EDITING_RISE;
			app->camera.resetCameraToEditorMode();
		}
		else {
			app->state = FLOATING;
			app->clearVertisesColor();
		}
	}
}
void Renderer::clearVertisesColor()
{
	Renderer* app = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
	for (auto& ver : app->vertices)
	{
		ver.color[1] = ver.color[2] = 1.0f;
	}
	app->refreshVertises();
}
void Renderer::refreshVertises() {
	swapChain.recreateSwapChain();		
	bufferManager.updateVertices(vertices);
}

bool near_equal(float a, float b, float epsilon)
{
	return (std::abs(a - b) < epsilon);
}

void Renderer::EditingHeights(double xpos, double ypos)
{
	Renderer* app = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
	float normalizedXpos = xpos / swapChain.swapChainExtent.width;
	float normalizedYpos = ypos / swapChain.swapChainExtent.height;

	glm::vec4 viewport = glm::vec4(0.0f, 0.0f, swapChain.swapChainExtent.width, swapChain.swapChainExtent.height);
	glm::vec3 unproj = glm::unProject(glm::vec3(xpos, ypos, 0.5f), glm::mat4(1.0f)*camera.GetViewMatrix(), camera.GetProjectionMatrix(), viewport);
	test = std::to_string(unproj.x) + ", " + std::to_string(unproj.y) + ", " + std::to_string(unproj.x);
	testTemp = std::wstring(test.begin(), test.end());
	for (auto& ver : app->vertices)
	{
		if (near_equal(unproj.x, ver.pos.x / 10.0f, app->editingSize) && near_equal(-unproj.y, ver.pos.y / 10.0f, app->editingSize)) {
			ver.color[1] = ver.color[2] = 0.0f;
		}
		else
		{
			ver.color[1] = ver.color[2] = 1.0f;
		}
	}
	app->refreshVertises();
}

void Renderer::cursorPosCallback(GLFWwindow * window, double xpos, double ypos)
{
	Renderer* app = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
	if (app->mousePressed && app->state == FLOATING)
	{
		if (app->firstMouse)
		{
			app->lastX = xpos;
			app->lastY = ypos;
			app->firstMouse = false;
		}

		float xoffset = xpos - app->lastX;
		float yoffset = app->lastY - ypos; // reversed since y-coordinates go from bottom to top
		app->camera.ProcessMouseMovement(xoffset, yoffset, false);
	}
	/*else if (app->mousePressed)
	{
		app->ProcessClick(xpos, ypos);
	}*/
	else if (!app->mousePressed && (app->state != FLOATING))
		app->EditingHeights(xpos, ypos);
	app->lastX = xpos;
	app->lastY = ypos;
}

void Renderer::ProcessClick(double xpos, double ypos)
{
	//Tutaj obs³uga klikniecia w trybie edycji
	Renderer* app = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
	float normalizedXpos = xpos / swapChain.swapChainExtent.width;
	float normalizedYpos = ypos / swapChain.swapChainExtent.height;
	
	glm::vec4 viewport = glm::vec4(0.0f, 0.0f, swapChain.swapChainExtent.width, swapChain.swapChainExtent.height);
	glm::vec3 unproj = glm::unProject(glm::vec3(xpos, ypos, 0.5f), glm::mat4(1.0f)*camera.GetViewMatrix(), camera.GetProjectionMatrix(), viewport);
	test = std::to_string(unproj.x) + ", " + std::to_string(unproj.y) + ", " + std::to_string(unproj.x);
	testTemp = std::wstring(test.begin(), test.end());
	for (auto& ver : app->vertices)
	{
		if (near_equal(unproj.x,ver.pos.x/10.0f, app->editingSize) && near_equal(-unproj.y, ver.pos.y / 10.0f, app->editingSize)) {
			if(app->state == EDITING_RISE)
				ver.pos.z += app->editStrength;
			else
				ver.pos.z -= app->editStrength;
		}
	}
	app->refreshVertises();
}

void Renderer::mouseClickCallback(GLFWwindow * window, int button, int action, int mods)
{
	Renderer* app = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		app->mousePressed = true;
		if (app->state != FLOATING)
		{
			app->ProcessClick(app->lastX, app->lastY);
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		app->mousePressed = false;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		app->mousePressed = true;
		if (app->state != FLOATING)
		{
			if (app->state == EDITING_RISE)
				app->state = EDITING_FALL;
			else
				app->state = EDITING_RISE;
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
		app->mousePressed = false;
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
	ubo.view = camera.GetViewMatrix();
	ubo.proj = camera.GetProjectionMatrix();
	ubo.lightPosition = glm::vec3(10, 10, 10);  
	ubo.proj[1][1] *= -1;
	return ubo;
}



void Renderer::onWindowResized(GLFWwindow* window, int width, int height) {
	if (width == 0 || height == 0) return;
	Renderer* app = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
	app->swapChain.recreateSwapChain();
	app->bufferManager.initCommandBuffers();
}

std::vector<Vertex> Renderer::getVertices()
{
	return vertices;
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