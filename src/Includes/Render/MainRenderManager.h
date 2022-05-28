#pragma once

//#include <glad/glad.h>
//#include <glfw3/glfw3.h>
//#include <glm/glm.hpp>

#include <Render/PerspectiveCamera.h>
#include <Render/MazeRenderer.h>
#include <Render/RenderManager.h>
#include <Maze/Maze.h>
#include <Maze/MazePath.h>

#include <iostream>
#include <memory>
#include <vector>

//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

class MainRenderManager : public RenderManager
{
public:
	MainRenderManager(int width, int height, std::shared_ptr<Maze> initialMaze);
	~MainRenderManager();
	void setup();
	void render();
	void framebufferSizeCallback(GLFWwindow* window, int width, int height);
	GLFWwindow* getWindow();
	int getWidth();
	int getHeight();
	void setMazeUsing(std::shared_ptr<Maze> maze);
	std::shared_ptr<Camera> getCamera() override;

	glm::mat4 projection;
	PerspectiveCamera camera;
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
	std::shared_ptr<Maze> maze;
};

