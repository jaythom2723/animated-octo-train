#include "display.h"

#include "program.h"
#include "shader.h"
#include "hooks.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

#include "glext.h"

extern Program* programReference;

extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLISPROGRAMPROC glIsProgram;
extern PFNGLISVERTEXARRAYPROC glIsVertexArray;

float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f
};

VertexArray* vao = nullptr;
VertexBuffer* vbo = nullptr;

LRESULT createBuffers(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
	vao = new VertexArray();
	VertexArray::Bind(vao->getId());

	vbo = new VertexBuffer(vertices);
	VertexBuffer::Bind(vbo->getId());

	if (glIsVertexArray(vao->getId())) {
		std::printf("Vertex Array Object is valid!\n");
	}
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	if (glIsProgram(programReference->getId())) {
		std::printf("Shader Program is valid!\n");
	}

	std::printf("Buffers have been created!\n");

	return 0;
}

LRESULT draw(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
	glClearColor(0.5f, 0.2f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	programReference->use();
	VertexArray::Bind(vao->getId());

	glDrawArrays(GL_TRIANGLES, 0, 3);
	
	SwapBuffers(GetDC(hwnd));

	return 0;
}

int main() {
	Display dsp("test", 0, 0, 640, 480);

	hooks::add(WM_CREATE, "createBuffers", &createBuffers);
	hooks::add(WM_PAINT, "drawPaint", &draw);

	dsp.loop();

	delete vao;
	delete vbo;

	return 0;
}

