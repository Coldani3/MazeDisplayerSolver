#include <Render/GUI/GUIRenderManager.h>
#include <Render/GUI/TwoDCamera.h>

GUIRenderManager::GUIRenderManager(std::shared_ptr<Maze> maze, int width, int height) {
	camera = TwoDCamera();
	fourDIndicator = std::make_unique<FourDLocationIndicatorRenderer>(camera, maze);
	this->maze = maze;
	ortho = glm::ortho(0.0f, (float) width, (float) height, 0.0f, 0.1f, 1.0f);
}

GUIRenderManager::~GUIRenderManager() {
	RenderManager::~RenderManager();
}

void GUIRenderManager::setup() {
	fourDIndicator->setup();
}

void GUIRenderManager::render() {
	fourDIndicator->render();
}

void GUIRenderManager::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	ortho = glm::ortho(0.0f, (float) width, (float) height, 0.0f, 0.1f, 1.0f);
	this->width = width;
	this->height = height;
}

std::shared_ptr<Camera> GUIRenderManager::getCamera() {
	return std::make_shared<TwoDCamera>(camera);
}
