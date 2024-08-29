#ifndef PROGRAM_H
#define PROGRAM_H

#include "shader.h"

#include <vector>

class Program {
private:
	int id;

	std::vector<Shader> attachedShaders;
public:
	Program();
	~Program();

	void attachShader(Shader& shader);
	void detachShader(Shader& shader);

	int getId();
	void link();
	void use();
};

#endif // PROGRAM_H