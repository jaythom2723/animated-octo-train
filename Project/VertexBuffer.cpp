#include <Windows.h>
#include <gl/GL.h>

#include "glext.h"

#include "VertexBuffer.h"

extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;

VertexBuffer::VertexBuffer(const float* data) {
	this->data = const_cast<float*>(data);

	glGenBuffers(1, &id);
	VertexBuffer::Bind(id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
	VertexBuffer::Unbind();
}

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &id);
}

unsigned int& VertexBuffer::getId() {
	return id;
}

const float* const VertexBuffer::getData() {
	return const_cast<const float* const>(data);
}

void VertexBuffer::Bind(unsigned int& id) {
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VertexBuffer::Unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}