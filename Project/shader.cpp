#include <Windows.h>
#include <GL/GL.h>

#include "glext.h"

#include "shader.h"

#include <fstream>
#include <sstream>

extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLDELETESHADERPROC glDeleteShader;

Shader::Shader(std::string filename, int type) {
	id = glCreateShader(type);
	this->type = type;

	std::string source = Shader::getShaderSource(filename);
	const char* const csrc = source.c_str();

	glShaderSource(id, 1, &csrc, nullptr);
	glCompileShader(id);

	int success;
	char infoLog[1024];
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(id, 1024, nullptr, infoLog);
		std::printf("%s\n", infoLog);
	}
}

Shader::~Shader() {
	glDeleteShader(id);
}

int& Shader::getId() {
	return id;
}

std::string Shader::getShaderSource(std::string filename) {
	std::ifstream file;
	std::stringstream ss;
	std::string line;

	file.open(filename);
	if (!file.is_open())
		return "";

	while (std::getline(file, line)) {
		ss << line << "\n";
	}

	return ss.str();
}