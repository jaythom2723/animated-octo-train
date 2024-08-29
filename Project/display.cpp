#define DEV
#include "display.h"

#include <cstdio>
#include <cstdlib>

#include "program.h"
#include "hooks.h"

Display* displayReference;
extern Program* programReference;

extern std::map<int, std::vector<HOOK>> attachedHooks;
extern std::map<HOOK, std::string> hookRegistry;

extern void loadDefaultMessageHooks();

LRESULT CALLBACK Display::WindowProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
	if (attachedHooks.find(umsg) == attachedHooks.end())
		return DefWindowProc(hwnd,umsg,wparam,lparam);


	std::vector<HOOK>& list = attachedHooks[umsg];
	for (int i = 0; i < list.size(); i++) {
		if (list[i](hwnd, umsg, wparam, lparam) > 0) {
			std::printf("Could not complete hook: %s\n", hookRegistry[list[i]].c_str());
			PostQuitMessage(0);
		}
	}

	return 0;
}

Display::Display(const char *title, int x, int y, int width, int height) {
	WNDCLASS wc = { 0 };

	instanceHandle = GetModuleHandle(nullptr);

	wc.lpfnWndProc = Display::WindowProc;
	wc.lpszClassName = L"GLWINDOW";
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
	wc.hInstance = instanceHandle;
	wc.style = CS_OWNDC;
	if (!RegisterClass(&wc))
		std::exit(-1);

	windowHandle = CreateWindowW(
		wc.lpszClassName,
		L"GL_TEST_WINDOW",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		x, y, width, height,
		NULL,
		NULL,
		instanceHandle,
		NULL
	);
	if (!windowHandle)
		std::exit(-2);

	loadDefaultMessageHooks();

	displayReference = this;
}

Display::~Display() {
	if(programReference != nullptr)
		delete programReference;

	programReference = nullptr;
	DestroyWindow(windowHandle);
	displayReference = nullptr;
	PostQuitMessage(0);
}

void Display::loop() {
	if (deviceContext == NULL || glContext == NULL) {
		SendMessage(windowHandle, WM_CREATE, 0, 0);
	}

	MSG msg = { };
	while (GetMessage(&msg, windowHandle, 0, 0) > 0) {
		DispatchMessageW(&msg);
	}
}

void Display::setDeviceContext(HDC context) {
	deviceContext = context;
}

void Display::setGLContext(HGLRC context) {
	glContext = context;
}