
#include "Shared.h"

#include "Renderer.h"

#include <stdio.h>
#include <tchar.h>
#include <atlstr.h>
#include <string>
#include <fstream>

#define WINDOW L"Vulkan Terrain Demo"

LRESULT CALLBACK WndProc(HWND mainWindow, UINT msg, WPARAM wParam, LPARAM lParam);

/** GUI window*/
HWND mainWindow;
/** Height map file path */
HWND filepath;
/** Information field */
HWND infoLabel;
/** Label showing FPS */
HWND fpsLabel;
/** Label showing the number of vertices */
HWND verticesLabel;
/** Label showing the number of indices */
HWND indicesLabel;
/** Height map file path as string */
std::string path;

HeightMap * heightMap;

Renderer * renderer;

/** Generic WinApi window, with placed widgets */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Failed to register the window.", L"Error", MB_ICONERROR | MB_OK);
		return EXIT_FAILURE;
	}

	mainWindow = CreateWindowEx(WS_EX_CLIENTEDGE, WINDOW, L"Vulkan Terrain Demo", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 400, NULL, NULL, hInstance, NULL);
	if (!mainWindow)
	{
		MessageBox(NULL, L"Failed to create window.", L"Error", MB_ICONERROR | MB_OK);
		return EXIT_FAILURE;
	}

	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	WCHAR defaultRatioX[5];
	swprintf_s(defaultRatioX, 5, L"%d", DEF_RATIO);
	WCHAR defaultRatioY[5];
	swprintf_s(defaultRatioY, 5, L"%d", DEF_RATIO);
	WCHAR defaultSize[5];
	swprintf_s(defaultSize, 5, L"%d", PATCH_SIZE);
	WCHAR defaultZScale[5];
	swprintf_s(defaultZScale, 5, L"%d", DEF_Z_SCALE);

	HWND openFileButton = CreateWindowEx(0, L"BUTTON", L"Open File...", WS_CHILD | WS_VISIBLE, 20, 20, 150, 30, mainWindow, (HMENU)OPEN_FILE_BUTTON, hInstance, NULL);
	filepath = CreateWindowEx(0, L"EDIT", L"", ES_READONLY | ES_AUTOHSCROLL | WS_CHILD | WS_VISIBLE, 180, 27, 600, 30, mainWindow, (HMENU)FILEPATH_LABEL, hInstance, NULL);

	HWND ratioLabel = CreateWindowEx(0, L"EDIT", L"Ratio (w to h):", ES_READONLY | WS_CHILD | WS_VISIBLE, 20, 60, 150, 30, mainWindow, (HMENU)RATIO_LABEL, hInstance, NULL);
	HWND ratioFieldX = CreateWindowEx(0, L"EDIT", defaultRatioX, WS_BORDER | ES_NUMBER | WS_CHILD | WS_VISIBLE, 180, 60, 100, 20, mainWindow, (HMENU)RATIO_FIELD_1, hInstance, NULL);
	HWND ratioFieldY = CreateWindowEx(0, L"EDIT", defaultRatioY, WS_BORDER | ES_NUMBER | WS_CHILD | WS_VISIBLE, 290, 60, 100, 20, mainWindow, (HMENU)RATIO_FIELD_2, hInstance, NULL);

	HWND sizeLabel = CreateWindowEx(0, L"EDIT", L"Size (vertices):", ES_READONLY | WS_CHILD | WS_VISIBLE, 20, 90, 150, 30, mainWindow, (HMENU)SIZE_LABEL, hInstance, NULL);
	HWND sizeField = CreateWindowEx(0, L"EDIT", defaultSize, WS_BORDER | ES_NUMBER | WS_CHILD | WS_VISIBLE, 180, 90, 100, 20, mainWindow, (HMENU)SIZE_FIELD, hInstance, NULL);


	HWND zScaleLabel = CreateWindowEx(0, L"EDIT", L"Z-Scale (1-255):", ES_READONLY | WS_CHILD | WS_VISIBLE, 20, 120, 150, 30, mainWindow, (HMENU)Z_SCALE_LABEL, hInstance, NULL);
	HWND zScaleField = CreateWindowEx(0, L"EDIT", defaultZScale, WS_BORDER | ES_NUMBER | WS_CHILD | WS_VISIBLE, 180, 120, 100, 20, mainWindow, (HMENU)Z_SCALE_FIELD, hInstance, NULL);

	fpsLabel = CreateWindowEx(0, L"EDIT", L"FPS: ", ES_READONLY | WS_CHILD | WS_VISIBLE, 20, 150, 300, 30, mainWindow, (HMENU)FPS_LABEL, hInstance, NULL);

	verticesLabel = CreateWindowEx(0, L"EDIT", L"Vertices: ", ES_READONLY | WS_CHILD | WS_VISIBLE, 20, 180, 300, 30, mainWindow, (HMENU)VERTICES_LABEL, hInstance, NULL);

	indicesLabel = CreateWindowEx(0, L"EDIT", L"Indices: ", ES_READONLY | WS_CHILD | WS_VISIBLE, 20, 210, 300, 30, mainWindow, (HMENU)INDICES_LABEL, hInstance, NULL);

	infoLabel = CreateWindowEx(0, L"EDIT", L"Ready", ES_READONLY | WS_CHILD | WS_VISIBLE, 20, 240, 300, 30, mainWindow, (HMENU)INFO_LABEL, hInstance, NULL);

	HWND applyButton = CreateWindowEx(0, L"BUTTON", L"RUN", WS_CHILD | WS_VISIBLE, 20, 270, 220, 30, mainWindow, (HMENU)APPLY_BUTTON, hInstance, NULL);

	HWND saveButton = CreateWindowEx(0, L"BUTTON", L"Save As...", WS_CHILD | WS_VISIBLE, 20, 310, 220, 30, mainWindow, (HMENU)SAVE_BUTTON, hInstance, NULL);

	EnableWindow(applyButton, FALSE);

	EnableWindow(saveButton, FALSE);

	ShowWindow(mainWindow, nCmdShow);
	UpdateWindow(mainWindow);

	MSG message;
	while (GetMessage(&message, NULL, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	return message.wParam;
}

/** Check for callbacks of GUI */
LRESULT CALLBACK WndProc(HWND mainWindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_COMMAND:
		switch (wParam)
		{
		case OPEN_FILE_BUTTON:
		{
			OPENFILENAME ofnOpen;
			ZeroMemory(&ofnOpen, sizeof(ofnOpen));
			ofnOpen.lStructSize = sizeof(ofnOpen);
			ofnOpen.hwndOwner = mainWindow;
			ofnOpen.lpstrFile = new TCHAR[MAX_PATH];
			ofnOpen.lpstrFile[0] = '\0';

			WCHAR initOpenPath[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, initOpenPath);
			ofnOpen.lpstrInitialDir = (LPCWSTR)initOpenPath;

			ofnOpen.nMaxFile = MAX_PATH;

			ofnOpen.lpstrFilter = TEXT("All Files (*.*)\0*.*\0BMP Files (*.bmp)\0*.bmp\0TGA Files (*.tga)\0*.tga\0PNG Files (*.png)\0*.png\0JPG Files (*.jpg)\0*.jpg\0");
			ofnOpen.nFilterIndex = 0;

			ofnOpen.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			if (GetOpenFileName(&ofnOpen)) {
				SetDlgItemText(mainWindow, FILEPATH_LABEL, ofnOpen.lpstrFile);
				SetCursor(LoadCursor(NULL, IDC_WAIT));
				path = CW2A(ofnOpen.lpstrFile);
				EnableWindow(GetDlgItem(mainWindow, APPLY_BUTTON), TRUE);
				SetCursor(LoadCursor(NULL, IDC_ARROW));
			}
			else
			{
				break;
			}

			SetDlgItemText(mainWindow, INFO_LABEL, L"File ready");

			break;
		}
		case APPLY_BUTTON:
		{
			SetCursor(LoadCursor(NULL, IDC_WAIT));

			UINT ratioW = GetDlgItemInt(mainWindow, RATIO_FIELD_1, NULL, TRUE);
			UINT ratioH = GetDlgItemInt(mainWindow, RATIO_FIELD_2, NULL, TRUE);
			if (ratioW > 99999 || ratioW < 0 || ratioH > 99999 || ratioH < 0) {
				MessageBox(NULL, L"Please choose ratio value between 0 and 99999", L"Error", MB_ICONERROR | MB_OK);
				break;
			}
			UINT size = GetDlgItemInt(mainWindow, SIZE_FIELD, NULL, TRUE);
			if (size > 4096 || size < 1) {
				MessageBox(NULL, L"Please choose size between 1 and 4096", L"Error", MB_ICONERROR | MB_OK);
				break;
			}

			UINT zScale = GetDlgItemInt(mainWindow, Z_SCALE_FIELD, NULL, TRUE);
			if (zScale > 255 || zScale < 1) {
				MessageBox(NULL, L"Please choose z-scale between 1 and 255", L"Error", MB_ICONERROR | MB_OK);
				break;
			}
			
			double duration;

			SetDlgItemText(mainWindow, INFO_LABEL, L"Running");

			float ratio;
			if (ratioH == 0) ratio = 0;
			else ratio = (float)ratioW / (float)ratioH;
			heightMap = new HeightMap(path, size, ratio, zScale);
			renderer = new Renderer(heightMap->generateVertices(), heightMap->generateIndices(), TEXTURE_SOURCE);
			float i = heightMap->getHeight(0, 0);
			try {
				EnableWindow(GetDlgItem(mainWindow, APPLY_BUTTON), FALSE);
				EnableWindow(GetDlgItem(mainWindow, SAVE_BUTTON), TRUE);
				renderer->run(mainWindow);
			}
			catch (const std::runtime_error& e) {
				MessageBox(NULL, LPCWSTR(e.what()), L"Error", MB_ICONERROR | MB_OK);
				std::cerr << e.what() << std::endl;
				return EXIT_FAILURE;
			}

			EnableWindow(GetDlgItem(mainWindow, APPLY_BUTTON), TRUE);
			EnableWindow(GetDlgItem(mainWindow, SAVE_BUTTON), FALSE);

			SetDlgItemText(mainWindow, INFO_LABEL, L"Stopped");
			SetDlgItemText(mainWindow, FPS_LABEL, L"FPS: -");

			SetDlgItemText(mainWindow, VERTICES_LABEL, L"Vertices: -");
			SetDlgItemText(mainWindow, INDICES_LABEL, L"Indices: -");

			SetCursor(LoadCursor(NULL, IDC_ARROW));
			break;
		}
		case SAVE_BUTTON: 
		{
			OPENFILENAME ofnSave;
			ZeroMemory(&ofnSave, sizeof(ofnSave));
			ofnSave.lStructSize = sizeof(ofnSave);
			ofnSave.hwndOwner = mainWindow;
			ofnSave.lpstrFile = new TCHAR[MAX_PATH];
			ofnSave.lpstrFile[0] = '\0';

			WCHAR initSavePath[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, initSavePath);
			ofnSave.lpstrInitialDir = (LPCWSTR)initSavePath;

			ofnSave.nMaxFile = MAX_PATH;

			ofnSave.lpstrFilter = TEXT("BMP Files (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0");
			ofnSave.nFilterIndex = 0;

			ofnSave.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

			if (GetSaveFileName(&ofnSave)) {
				
				SetCursor(LoadCursor(NULL, IDC_WAIT));
				
				std::string outPath = CW2A(ofnSave.lpstrFile);

				if (outPath.substr(outPath.length() - 4) != ".bmp" && outPath.substr(outPath.length() - 4) != ".BMP")
					outPath += ".bmp";

				SetCursor(LoadCursor(NULL, IDC_ARROW));
				
				heightMap->setVertices(renderer->getVertices());
				
				heightMap->saveAsImage(outPath);
			}
			else
			{
				break;
			}

			

			SetDlgItemText(mainWindow, INFO_LABEL, L"File saved");
		}
		}
		break;
	case WM_CLOSE:
		DestroyWindow(mainWindow);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(mainWindow, msg, wParam, lParam);
	}
	return 0;
}

/*
int main()
{
	
	HeightMap heightMap("test.bmp", PATCH_SIZE, DEF_RATIO, DEF_Z_SCALE);
	Renderer renderer(heightMap.generateVertices(), heightMap.generateIndices(), TEXTURE_SOURCE);
	float i = heightMap.getHeight(0, 0);
	try {
		
		renderer.run(0);
	}
	catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	

	return 0;

}*/

