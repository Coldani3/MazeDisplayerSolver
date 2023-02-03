#include "GUIRenderManager.h"
#include "TwoDCamera.h"

GUIRenderManager::GUIRenderManager(std::shared_ptr<Maze> maze, int width, int height) {
	camera = std::make_shared<TwoDCamera>(width, height);
	fourDIndicator = std::make_unique<FourDLocationIndicatorRenderer>(camera, maze);
	this->maze = maze;
}

GUIRenderManager::~GUIRenderManager() {
	RenderManager::~RenderManager();
}

void GUIRenderManager::setup() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	fourDIndicator->setup();
}

void GUIRenderManager::render() {
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	fourDIndicator->render(mazeRenderInfo);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void GUIRenderManager::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	this->width = width;
	this->height = height;
	camera->updateSizes(width, height);
	fourDIndicator->updatePosition();
}

std::shared_ptr<Camera> GUIRenderManager::getCamera() const {
	return camera;
}
