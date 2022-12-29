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
#include <Render/MazeRenderInfo.h>
#include "../InputManager.h"

class Renderer {
public:
	/*
	 * Perform your OpenGL rendering setup (instantiating buffer objects, VAOs, shaders, shader programs etc.) as
	 * well as your other setup needed for this renderer.
	 */
	virtual void setup() = 0;
	/*
	 * Render what this renderer is meant to render, as well as update render logic.
	 */
	virtual void render(std::shared_ptr<MazeRenderInfo> mazeRenderInfo) = 0;
	/*
	 * Cleanup your OpenGL stuff here. You can use deleteProgramIfExists to help with this, especially given you will
	 * probably want to call this in your destructor to stay in line with RAII but also potentially manually call it.
	 */
	virtual void cleanup() = 0;

	virtual std::shared_ptr<Camera> getCamera() const = 0;

	/*
	 * This will be called once a frame. Use this to perform non performance intensive key checks with glfwGetKey(),
	 * such as to make sure you get input every frame. Performance issues here *will* cause FPS drops.
	 */
	virtual void getRenderPollInput(GLFWwindow* window, double delta, const InputManager& inputManager) = 0;

    void checkShaderCompileSuccess(unsigned int shader);
	void checkProgramCompileSuccess(unsigned int program);

	void deleteProgramIfExists(unsigned int program, std::string name);
};

