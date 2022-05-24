#include <Render/GUI/GUIRenderManager.h>

GUIRenderManager::GUIRenderManager(Maze maze) : RenderManager() {
	this->maze = maze;
}

GUIRenderManager::~GUIRenderManager() {
	RenderManager::~RenderManager();
}

void GUIRenderManager::setup() {

}

void GUIRenderManager::render() {

}