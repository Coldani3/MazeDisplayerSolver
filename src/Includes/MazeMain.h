#ifndef __gl_h
#include <glad/glad.h>
#endif

#include <glfw3/glfw3.h>
#include <memory>

#include "Render/MainRenderManager.h"
#include "Render/GUI/GUIRenderManager.h"
#include "Maze/Maze.h"
#include "AI/AIManager.h"

#pragma once
class MazeMain
{
//public:
	bool running = true;
	std::shared_ptr<MainRenderManager> threeDRenderer = nullptr;
	std::shared_ptr<GUIRenderManager> guiRenderer = nullptr;
	std::shared_ptr<Maze> maze;
	std::unique_ptr<AIManager> aiManager;
	std::shared_ptr<Window> window;
	int windowXPos, windowYPos, windowWidth, windowHeight;


public:
	int main();

	void loadMaze();
	void setupGLFW();
	void setupMonitor();
	void setupRenderers();
};

