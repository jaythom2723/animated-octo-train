#ifndef SHADER_H
#define SHADER_H

#include <string>

class Shader {
private:
	int id;
	int type;

	static std::string getShaderSource(std::string filename);
public:
	Shader(std::string filename, int type);
	~Shader();

	int& getId();

	bool operator==(Shader shader) {
		return this->id == shader.getId();
	}
};

#endif // SHADER_H