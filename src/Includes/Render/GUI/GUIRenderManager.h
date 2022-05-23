#include <Maze/Maze.h>

#pragma once
class GUIRenderManager {
	GUIRenderManager(Maze maze);
	~GUIRenderManager();

	void setup();
	void draw();
};

