#pragma once

#include <glad/glad.h>
#include <glfw3/glfw3.h>
#include <glm/glm.hpp>

#include <Render/Camera.h>
#include <Render/MazeRenderer.h>
#include <Maze/Maze.h>
#include <Maze/MazePath.h>

#include <iostream>
#include <memory>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class RenderManager
{
public:
	RenderManager(int width, int height, Maze initialMaze);
	~RenderManager();
	void setup();
	void draw();
	void framebufferSizeCallback(GLFWwindow* window, int width, int height);
	GLFWwindow* getWindow();
	int getWidth();
	int getHeight();
	void setMazeUsing(Maze maze);
	std::shared_ptr<Camera> getCamera();

	glm::mat4 projection;
	std::shared_ptr<Camera> camera;
	std::unique_ptr<MazeRenderer> mazeRenderer;
	bool showPath = true;

	float centerX = 500.0f;
	float centerY = 500.0f;
	float centerZ = 500.0f;
private:
	GLFWwindow* window;
	int width;
	int height;
	int defaultWidth;
	int defaultHeight;
	int currentW = 0;
	Maze maze;
};

