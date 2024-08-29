#include <Windows.h>
#include <gl/GL.h>

#include "glext.h"
#include "wglext.h"

#include "hooks.h"
#include "program.h"

#include <map>
#include <vector>

#define ISREFVALID(ref) if(ref == nullptr) std::exit(-3);

extern Display* displayReference;
extern Program* programReference;

extern void loadGLFunctions();
extern void loadGLUtilityFunctions();
extern void glExtensionCheck();
extern void wglGetExtensions(HWND hwnd);

extern PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;

extern bool WGL_CREATE_CONTEXT_ARB;

// hooks that have been created
std::map<int, std::vector<HOOK>> attachedHooks = {};

// keeps track of all registered hook names for the use of retrieving hook functions
std::map<HOOK, std::string> hookRegistry = {};

//void Display::removeMessageHook(int message, HOOK hook) {
//	// currently selected list of hooks
//	std::vector<HOOK>& hookList = Display::messageHooks[message];
//	std::vector<HOOK>::iterator iter;
//
//	// if the hook doesn't exist already, do nothing
//	iter = std::find(hookList.begin(), hookList.end(), hook);
//	bool hookExists = iter != hookList.end();
//	if (!hookExists)
//		return;
//
//	Display::messageHooks[message].erase(iter);
//}

void hooks::add(int msg, std::string name, HOOK func) {
	// check if the message vector exists, if not, create it.
	if (attachedHooks.find(msg) == attachedHooks.end())
		attachedHooks.insert({msg, { }});

	// check if the hook already exists, if not, create it.
	std::vector<HOOK>& list = attachedHooks[msg];
	std::vector<HOOK>::iterator iter;
	iter = std::find(list.begin(), list.end(), func);
	if (iter != list.end()) // found the hook, do nothing
		return;

	hookRegistry.insert({ func, name });
	list.push_back(func);
}

HOOK hooks::get(std::string name) {
	for (std::pair<HOOK, std::string> kv : hookRegistry) {
		if (kv.second == name) {
			std::printf("FOUND A VALID HOOK! %s\n", kv.second.c_str());
			return kv.first;
		}
	}

	return nullptr;
}

// hook definitions
LRESULT createOpenGLContext(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
LRESULT initShaders(HWND, UINT umsg, WPARAM wparam, LPARAM lparam);

void loadDefaultMessageHooks() {
	hooks::add(WM_CREATE, "OpenGLInit", &createOpenGLContext);
	hooks::add(WM_CREATE, "OpenGLShaderInit", &initShaders);
}

LRESULT createOpenGLContext(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
	ISREFVALID(displayReference);

	PIXELFORMATDESCRIPTOR pfd;
	int nPixelFormat;
	BOOL bResult;
	HGLRC tempContext, renderContext;
	HDC hdc;
	int attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 1,
		WGL_CONTEXT_FLAGS_ARB, 0,
		0
	};

	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	hdc = GetDC(hwnd);
	nPixelFormat = ChoosePixelFormat(hdc, &pfd);
	if (nPixelFormat == 0) {
		return 1;
	}

	bResult = SetPixelFormat(hdc, nPixelFormat, &pfd);
	if (!bResult) {
		return 1;
	}

	tempContext = wglCreateContext(hdc);
	wglMakeCurrent(hdc, tempContext);

	loadGLUtilityFunctions();
	wglGetExtensions(hwnd);
	glExtensionCheck();
	loadGLFunctions();
	
	if (WGL_CREATE_CONTEXT_ARB) {
		renderContext = wglCreateContextAttribsARB(hdc, NULL, attribs);
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(tempContext);
		wglMakeCurrent(hdc, renderContext);
	}
	else {
		renderContext = tempContext;
	}

	displayReference->setDeviceContext(hdc);
	displayReference->setGLContext(renderContext);

	RECT wdim;
	GetWindowRect(hwnd, &wdim);

	glViewport(0, 0, wdim.right, wdim.bottom);

	std::printf("\nOpenGL Context Successfully Created!\n");

	return 0;
}

LRESULT initShaders(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
	ISREFVALID(displayReference);

	programReference = new Program();
	ISREFVALID(programReference);

	Shader vertex("vertex.glsl", GL_VERTEX_SHADER);
	Shader fragment("fragment.glsl", GL_FRAGMENT_SHADER);

	programReference->attachShader(vertex);
	programReference->attachShader(fragment);
	programReference->link();
	programReference->use();

	std::printf("Shaders have been initialized!\n");

	return 0;
}