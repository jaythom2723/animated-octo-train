#ifndef DISPLAY_H
#define DISPLAY_H

#include <Windows.h>
#include <gl/GL.h>

#include <map>
#include <string>
#include <vector>

class Display
{
private:
	HINSTANCE instanceHandle;
	HWND windowHandle;
	HDC deviceContext = NULL;
	HGLRC glContext = NULL;

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

public:
	Display(const char *title, int x, int y, int width, int height);
	~Display();

	void loop();

	void setDeviceContext(HDC context);
	void setGLContext(HGLRC context);
};

#endif // DISPLAY_H