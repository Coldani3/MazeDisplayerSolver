#include "Render/ShaderProgram.h"

ShaderProgram& ShaderProgram::loadShader(const char* shaderCode, unsigned int& shaderAddress, int shaderType) {
	if (!created) {
		shaderAddress = glCreateShader(shaderType);
		glShaderSource(shaderAddress, 1, &shaderCode, NULL);
		glCompileShader(shaderAddress);
		checkShaderCompileSuccess(shaderAddress);
		createdShaders.push_back(shaderAddress);
	}

	return *this;
}

void ShaderProgram::checkShaderCompileSuccess(unsigned int shader) {
	int success;
	char infoLog[512];

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cerr << "SHADER ERROR: " << infoLog << std::endl;
	}
}

ShaderProgram::ShaderProgram(std::string name) {
	this->name = name;
}

ShaderProgram::ShaderProgram(ShaderProgram&& moving) noexcept {
	vertShaderAddress = std::exchange(moving.vertShaderAddress, 0);
	fragShaderAddress = std::exchange(moving.fragShaderAddress, 0);
	program = std::exchange(moving.program, 0);
	createdShaders = std::move(moving.createdShaders);
	created = std::move(moving.created);
	name = std::move(moving.name);
}

ShaderProgram::ShaderProgram(const ShaderProgram& copying) noexcept {
	vertShaderAddress = copying.vertShaderAddress;
	fragShaderAddress = copying.fragShaderAddress;

	program = copying.program;
	createdShaders = copying.createdShaders;
	created = copying.created;
	name = copying.name;
}

ShaderProgram& ShaderProgram::loadVertexShader(const char* vertShader) {
	return loadShader(vertShader, vertShaderAddress, GL_VERTEX_SHADER);
}

ShaderProgram& ShaderProgram::loadFragmentShader(const char* fragShader) {
	// TODO: insert return statement here
	return loadShader(fragShader, fragShaderAddress, GL_FRAGMENT_SHADER);
}

ShaderProgram& ShaderProgram::createProgram() {
	program = glCreateProgram();

	for (unsigned int shader : createdShaders) {
		glAttachShader(program, shader);
	}

	glLinkProgram(program);

	//TODO: Revisit this in case sharing the same shader across multiple programs is ideal
	for (unsigned int shader : createdShaders) {
		glDeleteShader(shader);
	}

	created = true;

	return *this;
}

ShaderProgram::~ShaderProgram() {
	int programSuccess;
	glGetProgramiv(program, GL_DELETE_STATUS, &programSuccess);

	if (programSuccess != GL_TRUE) {
		std::cout << "Cleaning up shader program " << name << "..." << '\n';
		glDeleteProgram(program);
	} else {
		std::cout << "Shader program " << name << " already cleaned up!" << '\n';
	}
}

unsigned int ShaderProgram::getProgram() const {
	return program;
}

unsigned int ShaderProgram::getVertShader() const {
	return vertShaderAddress;
}

unsigned int ShaderProgram::getFragShader() const {
	return fragShaderAddress;
}
