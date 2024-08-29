#include "VertexArray.h"

#include <Windows.h>
#include <gl/GL.h>

#include "glext.h"

extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;

VertexArray::VertexArray() {
	glGenVertexArrays(1, &id);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &id);
}

unsigned int& VertexArray::getId() {
	return id;
}

void VertexArray::Bind(unsigned int& id) {
	glBindVertexArray(id);
}

void VertexArray::Unbind() {
	glBindVertexArray(0);
}