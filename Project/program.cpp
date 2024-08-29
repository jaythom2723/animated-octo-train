#include <Windows.h>
#include <gl/GL.h>
#include "glext.h"

#include "program.h"

extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLUSEPROGRAMPROC glUseProgram;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;

extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLDETACHSHADERPROC glDetachShader;
extern PFNGLDELETESHADERPROC glDeleteShader;

Program* programReference;

Program::Program() {
	id = glCreateProgram();
	programReference = this;
}

Program::~Program() {
	glDeleteProgram(id);
}

void Program::attachShader(Shader& shader) {
	// shader already attached, do nothing
	std::vector<Shader>::iterator iter = std::find(attachedShaders.begin(), attachedShaders.end(), shader);
	bool isAttached = iter != attachedShaders.end();
	if (isAttached)
		return;
	
	attachedShaders.push_back(shader);
	glAttachShader(id, shader.getId());
}

void Program::detachShader(Shader& shader) {
	std::vector<Shader>::iterator iter;

	// shader is already detached or has never been attached, do nothing
	iter = std::find(attachedShaders.begin(), attachedShaders.end(), shader);
	bool isAttached = iter != attachedShaders.end();
	if (!isAttached)
		return;
	
	glDetachShader(id, shader.getId());
	attachedShaders.erase(iter);
}

int Program::getId() {
	return id;
}

void Program::link() {
	glLinkProgram(id);

	int success;
	char infoLog[1024];
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 1024, nullptr, infoLog);
		std::printf("%s\n", infoLog);
		PostQuitMessage(-10);
	}

	for (int i = 0; i < attachedShaders.size(); i++) {
		std::vector<Shader>::iterator iter = std::find(attachedShaders.begin(), attachedShaders.end(), attachedShaders[i]);
		glDetachShader(id, attachedShaders.at(i).getId());
		attachedShaders.erase(iter);
	}
}

void Program::use() {
	glUseProgram(id);
}
