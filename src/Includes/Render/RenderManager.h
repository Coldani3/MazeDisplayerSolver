#include <glad/glad.h>
#include <glfw3/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Maze/Maze.h>

#pragma once
class RenderManager {
public:
	RenderManager();
	~RenderManager();
	virtual void setup();
	virtual void render();
};

