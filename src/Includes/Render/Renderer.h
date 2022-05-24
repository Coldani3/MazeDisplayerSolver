#pragma once
#include <glad/glad.h>
#include <glfw3/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Render/Shaders.h>

class Renderer {
public:
	virtual void setup();
	virtual void render();
	virtual void cleanup();

    void checkShaderCompileSuccess(unsigned int shader);
	void checkProgramCompileSuccess(unsigned int program);
};

