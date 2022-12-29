#include <glad/glad.h>
#include <glfw3/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Maze/Maze.h>
#include <Render/MazeRenderInfo.h>
#include <Render/PerspectiveCamera.h>

#pragma once
class RenderManager {
public:
	virtual void setup() = 0;
	virtual void render() = 0;
	std::shared_ptr<MazeRenderInfo> mazeRenderInfo;

	virtual std::shared_ptr<Camera> getCamera() const = 0;
};

