#ifndef __gl_h
#include <glad/glad.h>
#endif

#include <glfw3/glfw3.h>
#include <memory>
#include <string>
#include <thread>

#include "Render/MainRenderManager.h"
#include "Render/GUI/GUIRenderManager.h"
#include "Maze/Maze.h"
#include "Maze/MazePathManager.h"
#include "AI/AIManager.h"


#pragma once
class MazeMain
{
//public:
	bool running = true;
	std::shared_ptr<MainRenderManager> threeDRenderer = nullptr;
	std::shared_ptr<GUIRenderManager> guiRenderer = nullptr;
	std::shared_ptr<Maze> maze;
	std::shared_ptr<AIManager> aiManager;
	std::shared_ptr<Window> window;
	std::shared_ptr<MazePathManager> mazePathManager;
	int windowXPos, windowYPos, windowWidth, windowHeight;


public:
	int main();

	void loadMaze();
	//TEMP: Move me to some other file or object.
	//TODO: should this be a smart pointer? I feel like maybe it's better to get this as a ref and let the calling method
	//do what it wants
	std::shared_ptr<Maze> loadMazeFromFile(std::string path);
	void setupGLFW() noexcept;
	void setupMonitor() noexcept;
	void setupRenderers() noexcept;

	//TODO: move into Window?
	bool checkWindowInitialised(const Window& window);
	bool initialiseGLAD();

	void setupViewport();
	void setupFramebufferCallback();

	void framebufferSizeCallback(GLFWwindow* window, int width, int height);
};
