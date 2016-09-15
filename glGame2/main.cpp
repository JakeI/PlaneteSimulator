#include "StdAfx.h"

#include "shaders\LoadShaderProg.h"
#include "globals.h"
#include "Renderer.h"

#define HORIZON

#define DEFAULT_INPUT_PATH "inputPlanete.txt"
#ifdef BALE
#undef DEFAULT_INPUT_PATH
#define DEFAULT_INPUT_PATH	"inputPlanet.txt"
#endif
#ifdef SOLAR_SYSTEM
#undef DEFAULT_INPUT_PATH
#define DEFAULT_INPUT_PATH	"keplerModle.txt"
#endif
#ifdef HORIZON
#undef DEFAULT_INPUT_PATH
#define DEFAULT_INPUT_PATH	"inputPlanetHorizen.txt"
#endif
#ifdef CRESH
#undef DEFAULT_INPUT_PATH
#define DEFAULT_INPUT_PATH "inputCresh.txt"
#endif

DWORD WINAPI outputHandler(LPVOID lpParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
void registerWindowClass(HINSTANCE* hInstance);
HWND generateWindow(int width, int hight, HINSTANCE* hInstance);
HDC setPixelFormatStruct(HWND hWnd);
void releaseWgl(HWND hWnd, HDC hDC, HGLRC hRC);
float updateFPS(long long frameTicks);
float clock2MSf(clock_t ticks);
void initPlaeteFilePath(const char* cmd, int len);

bool running = true;

UserInput* userInput;
INPUT_CHANGE_BUFFER inputChangeBuffer;

char* planeteFilePath;

struct OutputParam {
	INPUT_CHANGE_BUFFER* changeID;
	UserInput* userInput;
	bool* running;
	HWND hWnd;
};

TRACKMOUSEEVENT tme;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {

	std::string cmd((char*)&pCmdLine);
	initPlaeteFilePath(cmd.c_str(), cmd.length());

	registerWindowClass(&hInstance);

	userInput = new UserInput();
	userInput->windowSize.width = 640;
	userInput->windowSize.hight = 480;
	inputChangeBuffer = CHANGE_WINDOW_SIZE | CHANGE_MOUSE_CORDINATES |
		CHANGE_MOUSE_BUTTON_STATE | CHANGE_KEYBOARD_STATE;

	HWND hWnd = generateWindow(userInput->windowSize.width, userInput->windowSize.hight, &hInstance);

	//Create Threads
	HANDLE hThread;

	OutputParam* outputParam = (OutputParam*)HeapAlloc(GetProcessHeap(), 
		HEAP_ZERO_MEMORY, sizeof(OutputParam));
	outputParam->changeID = &inputChangeBuffer;
	outputParam->running = &running;
	outputParam->userInput = userInput;
	outputParam->hWnd = hWnd;

	hThread = CreateThread(NULL, 0, outputHandler, (void*)outputParam, 0, NULL);

	if (hThread == NULL) {
		MessageBox(NULL, "Couldn't create threads", "ERROR", MB_OK);
	}

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	WaitForMultipleObjects(1, &hThread, TRUE, INFINITE);

	CloseHandle(hThread);
	if (outputParam != NULL) {
		HeapFree(GetProcessHeap(), 0, outputParam);
		outputParam = NULL;
	}

	delete userInput;

	return 0;
}

DWORD WINAPI outputHandler(LPVOID lpParam) {
	OutputParam* outputParam = (OutputParam*)lpParam;

	HDC hDC = setPixelFormatStruct(outputParam->hWnd);

	HGLRC hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);

	if (glewInit() != GLEW_OK)
		MessageBox(NULL, "Failed to initialize GLEW", "ERROR", MB_OK);

	initGlobals();
	Renderer* renderer = new Renderer(planeteFilePath);
	
	const int fps = 60;
	const int cpf = CLOCKS_PER_SEC / fps;
	int renderNextFrame = 0;
	long long lastFrameTime = 0;

	float lastVitualTimeMS = clock2MSf(clock());

	renderer->updateUserInput(outputParam->userInput, *outputParam->changeID);
	renderer->increment(clock2MSf(clock()), 0);
	renderer->draw();
	
	while (*outputParam->running) {

		int time = (int)(clock());
		if (time >= renderNextFrame) {
			renderNextFrame = clock() + cpf;
			float framerate = 1.0f / updateFPS(clock() - lastFrameTime); // Not used yet
			lastFrameTime = clock();

			SwapBuffers(hDC);

			renderer->updateUserInput(outputParam->userInput, *outputParam->changeID);
			*outputParam->changeID = 0;
			float currentVirtualTimeMS = clock2MSf(clock());
			renderer->increment(clock2MSf(clock()),
				currentVirtualTimeMS - lastVitualTimeMS);
			lastVitualTimeMS = currentVirtualTimeMS;
			renderer->draw();
		}
		else if (time <= renderNextFrame - 3) {
			Sleep(2);
		}
	}

	deleteGlobals();
	releaseWgl(outputParam->hWnd, hDC, hRC);

	delete renderer;
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	static bool mouseOutside = false;
	switch (Msg) {
	case WM_CREATE:
		
		/*while (ShowCursor(false) >= 0);
		SetCapture(hWnd);
		POINT p;
		p.x = userInput->windowSize.width / 2;
		p.y = userInput->windowSize.hight / 2;
		ClientToScreen(hWnd, &p);
		SetCursorPos(p.x, p.y);*/

			
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = hWnd;
		tme.dwHoverTime = 0;

		TrackMouseEvent(&tme);

		break;
	case WM_SIZE:
		userInput->windowSize.hight = HIWORD(lParam);
		userInput->windowSize.width = LOWORD(lParam);
		inputChangeBuffer |= CHANGE_WINDOW_SIZE;
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			SendMessage(hWnd, WM_QUIT, NULL, NULL);
			break;
		} else if (wParam == VK_F2) {
			int h = userInput->windowSize.hight, w = userInput->windowSize.width;
			if (userInput->windowSize.hight <= 520 && userInput->windowSize.hight >=440
				&& userInput->windowSize.width <= 680 && userInput->windowSize.width >= 600) {
				SetWindowPos(hWnd, HWND_TOP,
					0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)-30,
					SWP_SHOWWINDOW);
			} else {
				SetWindowPos(hWnd, HWND_TOP,
					(GetSystemMetrics(SM_CXSCREEN) - 640) / 2,
					(GetSystemMetrics(SM_CYSCREEN) - 480) / 2,
					640, 480,
					SWP_SHOWWINDOW);
			}
		}
		userInput->keyboardState.setTrue((int)wParam);
		inputChangeBuffer |= CHANGE_KEYBOARD_STATE;
		break;
	case WM_KEYUP:
		userInput->keyboardState.setFalse((int)wParam);
		inputChangeBuffer |= CHANGE_KEYBOARD_STATE;
		break;
	case WM_MOUSEMOVE: //TODO: Make this work
	{
		if (mouseOutside) {
			mouseOutside = false;
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			ShowCursor(TRUE);
			TrackMouseEvent(&tme);
		}

		userInput->mouseCordinates.y = HIWORD(lParam);
		userInput->mouseCordinates.x = LOWORD(lParam);
		inputChangeBuffer |= CHANGE_MOUSE_CORDINATES;
	}
		break;
	case WM_MOUSELEAVE: // TODO: Make this work
		userInput->mouseButtoneStates.leftButtonPressed = false;
		userInput->mouseButtoneStates.rightButtonPressed = false;
		inputChangeBuffer |= CHANGE_MOUSE_BUTTON_STATE;
		mouseOutside = true;
		break;
	case WM_LBUTTONDOWN:
		userInput->mouseButtoneStates.leftButtonPressed = true;
		inputChangeBuffer |= CHANGE_MOUSE_BUTTON_STATE;
		break;
	case WM_LBUTTONUP:
		userInput->mouseButtoneStates.leftButtonPressed = false;
		inputChangeBuffer |= CHANGE_MOUSE_BUTTON_STATE;
		break;
	case WM_RBUTTONDOWN:
		userInput->mouseButtoneStates.rightButtonPressed = true;
		inputChangeBuffer |= CHANGE_MOUSE_BUTTON_STATE;
		break;
	case WM_RBUTTONUP:
		userInput->mouseButtoneStates.rightButtonPressed = false;
		inputChangeBuffer |= CHANGE_MOUSE_BUTTON_STATE;
		break;
	case WM_MOUSEWHEEL:
		userInput->mouseWheel += GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
		inputChangeBuffer |= CHANGE_MOUSEWHEEL_STATE;
		break;
	case WM_DESTROY: case WM_QUIT: case WM_CLOSE:

		PostQuitMessage(0);
		running = false;

		return 0;
	default:
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}
	return 0;
}

void registerWindowClass(HINSTANCE* hInstance) {
	WNDCLASSEX wcex = { sizeof(WNDCLASSEX), NULL, WndProc, NULL, NULL, *hInstance,
		LoadIcon(*hInstance, IDI_APPLICATION), LoadCursor(*hInstance, IDC_ARROW),
		(HBRUSH)CreateSolidBrush(RGB(255, 0, 0)), NULL, "frame", NULL };
	if (!RegisterClassEx(&wcex)) { MessageBox(NULL, "RegisterClass failed", "ERROR", MB_OK); }
}

HWND generateWindow(int width, int hight, HINSTANCE* hInstance) {
	HWND hWnd = CreateWindow("frame", "Planet 3D demo",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		(GetSystemMetrics(SM_CXSCREEN) - width) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - hight) / 2,
		width, hight, NULL, NULL, *hInstance, NULL);
	if (!hWnd) { MessageBox(NULL, "CreateWindowEx failed", "ERROR", MB_OK); return 0; }
	return hWnd;
}

HDC setPixelFormatStruct(HWND hWnd) {
	HDC hDC = GetDC(hWnd);
	PIXELFORMATDESCRIPTOR pfd;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
	pfd.cDepthBits = 32;
	pfd.cColorBits = 24;
	pfd.iPixelType = PFD_TYPE_RGBA;
	if (!SetPixelFormat(hDC, ChoosePixelFormat(hDC, &pfd), &pfd)) {
		MessageBox(NULL, "Error: SetPielFormat failed", "ERROR", MB_OK);
		return 0;
	}
	return hDC;
}

void releaseWgl(HWND hWnd, HDC hDC, HGLRC hRC) {
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	if (!ReleaseDC(hWnd, hDC)) {
		MessageBox(hWnd, "Error: ReleaseDC Failed", "ERROR", MB_OK);
	}
}

float updateFPS(long long frameTicks) {

	const int numSamples = 100;
	static long long index = 0;
	static float samples[numSamples] = { 0.0f };

	int i = index % numSamples;
	samples[i] = frameTicks / (float)CLOCKS_PER_SEC;

	float sum = 0.0f;
	for (int j = 0; j < numSamples; j++) {
		sum += samples[i];
	}
	return sum / numSamples;
}

const float ticksPerMS = CLOCKS_PER_SEC / 1000;
float clock2MSf(clock_t ticks) {
	return (float)ticks / ticksPerMS;
}

void initPlaeteFilePath(const char* cmd, int len) {
	if (cmd != nullptr) {
		//Check if file exists
		/*std::ifstream f(cmd, std::ios::in);
		if (f.is_open()) {
			strcpy_s(planeteFilePath, len, cmd);
			f.close();
			return;
		}*/
	}
	planeteFilePath = DEFAULT_INPUT_PATH;
}