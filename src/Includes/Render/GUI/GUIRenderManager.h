#include <Maze/Maze.h>
#include <Render/RenderManager.h>

#pragma once
class GUIRenderManager : public RenderManager {
	GUIRenderManager(Maze maze);
	~GUIRenderManager();

	void setup();
	void render();

public:
	Maze maze;
};

