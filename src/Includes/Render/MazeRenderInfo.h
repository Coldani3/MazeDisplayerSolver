#include <memory>
#include <Maze/Maze.h>

#pragma once
/*
* Object that stores information about the rendering of a specific maze. Used to synchronise what w each Renderer
* is viewing, as well as eventually other settings.
* TODO: Could this be a struct?
*/
class MazeRenderInfo {
public:
	int wViewing = 0;

	MazeRenderInfo(int startW);
};

