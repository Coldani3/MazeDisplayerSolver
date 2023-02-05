#ifndef CD3_MAZE_SHADER_PROGRAM
#define CD3_MAZE_SHADER_PROGRAM

#include <glad/glad.h>
#include <glfw3/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <string>


#pragma once
class ShaderProgram {
	unsigned int vertShaderAddress, fragShaderAddress, geometryShaderAddress;
	unsigned int program;
	std::vector<unsigned int> createdShaders;
	bool created;

	ShaderProgram& loadShader(const char* shaderCode, unsigned int& shaderAddress, int shaderType);
	void checkShaderCompileSuccess(unsigned int shader);

public:
	std::string name;

	ShaderProgram(std::string name);
	ShaderProgram(ShaderProgram&& moving) noexcept;
	//while the shader destructor checks if it has already been deleted, this could cause
	//problems with the other copies being used later, so do not copy.
	ShaderProgram(const ShaderProgram& copying) noexcept = delete;

	ShaderProgram& loadVertexShader(const char* vertShader);
	ShaderProgram& loadFragmentShader(const char* fragShader);
	ShaderProgram& loadGeometryShader(const char* geometryShader);
	//TODO: Tesselation, evaluation and compute shaders (OpenGL 4.0 and 4.3 respectively)
	//TODO: Is it necessary to add these shaders to this? They're from a newer version than ours so we can't use them anyways.
	ShaderProgram& createProgram();
	~ShaderProgram();

	constexpr unsigned int getProgram() const;
	constexpr unsigned int getVertShader() const;
	constexpr unsigned int getFragShader() const;
	constexpr unsigned int getGeometryShader() const;
	GLint getUniform(const std::string& uniform);
	void uniform(const std::string& uniformName, const glm::mat4& mat, int matCount = 1, GLboolean transpose = GL_FALSE);
	void uniform(const std::string& uniformName, const glm::mat3& mat, int matCount = 1, GLboolean transpose = GL_FALSE);
	void uniform(const std::string& uniformName, const glm::vec3& vec, int vecCount = 1);
	void uniform(const std::string& uniformName, const glm::ivec3& vec, int vecCount = 1);
	void uniform(const std::string& uniformName, const glm::vec4& vec, int vecCount = 1);
	void uniform(const std::string& uniformName, const glm::ivec4& vec, int vecCount = 1);
	void uniform(const std::string& uniformName, float num);
	void uniform(const std::string& uniformName, int num);
	void use();

	ShaderProgram& operator=(ShaderProgram&& moving) noexcept;
};
#endif