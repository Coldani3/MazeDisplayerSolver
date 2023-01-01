#include <glad/glad.h>
#include <glfw3/glfw3.h>
#include <iostream>
#include <vector>
#include <string>


#pragma once
class ShaderProgram {
	unsigned int vertShaderAddress, fragShaderAddress;
	unsigned int program;
	std::vector<unsigned int> createdShaders;
	bool created;

	ShaderProgram& loadShader(const char* shaderCode, unsigned int& shaderAddress, int shaderType);
	void checkShaderCompileSuccess(unsigned int shader);

public:
	std::string name;

	ShaderProgram(std::string name);
	ShaderProgram(ShaderProgram&& moving) noexcept;
	ShaderProgram(const ShaderProgram& copying) noexcept;

	ShaderProgram& loadVertexShader(const char* vertShader);
	ShaderProgram& loadFragmentShader(const char* fragShader);
	ShaderProgram& createProgram();
	~ShaderProgram();

	unsigned int getProgram() const;
	unsigned int getVertShader() const;
	unsigned int getFragShader() const;
};

