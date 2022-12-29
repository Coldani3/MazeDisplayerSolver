#pragma once

#include <Render/PerspectiveCamera.h>
#include <Render/MazeRenderer.h>
#include <Render/RenderManager.h>
#include <Maze/Maze.h>
#include <Maze/MazePath.h>
#include "Window.h"

#include <iostream>
#include <memory>
#include <vector>

class MainRenderManager : public RenderManager
{
public:
	MainRenderManager(std::shared_ptr<Window> window, std::shared_ptr<Maze> initialMaze, std::shared_ptr<MazePathManager> pathManager);
	~MainRenderManager();
	void setup();
	void render();
	void framebufferSizeCallback(GLFWwindow* window, int width, int height);
	//GLFWwindow* getWindow();
	int getWidth() const;
	int getHeight() const;
	void setMazeUsing(std::shared_ptr<Maze> maze);
	std::shared_ptr<Camera> getCamera() const override;

	glm::mat4 projection;
	std::shared_ptr<PerspectiveCamera> camera;
	std::unique_ptr<MazeRenderer> mazeRenderer;
	bool showPath = true;
	std::shared_ptr<Window> window;

	float centerX = 500.0f;
	float centerY = 500.0f;
	float centerZ = 500.0f;
private:
	//GLFWwindow* window;
	int width;
	int height;
	int defaultWidth;
	int defaultHeight;
	int currentW = 0;
	std::shared_ptr<Maze> maze;
};

