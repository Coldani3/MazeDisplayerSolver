#include "../../Maze/Maze.h"
#include "../RenderManager.h"
#include "FourDLocationIndicatorRenderer.h"

#pragma once
class GUIRenderManager : public RenderManager {
public:
	std::shared_ptr<Maze> maze;
	std::unique_ptr<FourDLocationIndicatorRenderer> fourDIndicator;

	int width, height;

	GUIRenderManager(std::shared_ptr<Maze> maze, int width, int height);
	~GUIRenderManager();

	void setup();
	void render();
	void framebufferSizeCallback(GLFWwindow* window, int width, int height);
	std::shared_ptr<Camera> getCamera() const override;

private:
	std::shared_ptr<TwoDCamera> camera;
};

