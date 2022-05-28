#pragma once
#include <glad/glad.h>
#include <glfw3/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <memory>

#include <Render/Shaders.h>
#include <Render/Camera.h>

class Renderer {
public:
	Renderer(std::shared_ptr<Camera> camera);
	virtual void setup() = 0;
	virtual void render() = 0;
	virtual void cleanup() = 0;

	virtual void setCamera(std::shared_ptr<Camera> camera);

    void checkShaderCompileSuccess(unsigned int shader);
	void checkProgramCompileSuccess(unsigned int program);

	void deleteProgramIfExists(unsigned int program, std::string name);

protected:
	std::shared_ptr<Camera> camera;
};

