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
#include "InputManager.h"


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
	std::shared_ptr<InputManager> inputManager;
	int windowXPos, windowYPos, windowWidth, windowHeight;
	double lastFrame, delta = 0, fps = 0, lastWShift = 0, lastSolverShift = 0;


public:
	int main();

	void loadMaze();
	//TEMP: Move me to some other file or object.
	//TODO: should this be a smart pointer? I feel like maybe it's better to get this as a ref and let the calling method
	//do what it wants
	void setupGLFW() noexcept;
	GLFWmonitor* setupMonitor() noexcept;
	void getScreenInfoForWindow(GLFWmonitor* monitor, int& windowWidth, int& windowHeight) noexcept;
	void setupRenderers() noexcept;

	bool initialiseGLAD();

	void setupViewport();
	void setupFramebufferCallback(const Window& window);

	void framebufferSizeCallback(GLFWwindow* window, int width, int height);
	void checkWindowCloseKeyPressed(const Window& window);
	void handleFourDManoeuvre(const Window& window);
	void handleSolverIndexControls(const Window& window);
	void handleRendererInput(const Window& window, std::shared_ptr<Maze> maze);
	void handleInput(const Window& window, std::shared_ptr<Maze> maze);

	void renderLoop();
};

