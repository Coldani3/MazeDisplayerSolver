#pragma once
#include <Render/Renderer.h>
#include <Render/GUI/TwoDCamera.h>
#include <Maze/Maze.h>

class GUIRenderer : public Renderer {
public:
	GUIRenderer(std::shared_ptr<TwoDCamera> camera);

	//used for starting coords so we can have GUI components relative to the screen but keep ortho
	glm::vec3 screenRelativeCoords(float x, float y);

private:
};

