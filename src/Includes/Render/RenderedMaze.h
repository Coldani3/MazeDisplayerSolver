#include <Maze/Maze.h>
#include <Render/RenderManager.h>

#pragma once
class RenderedMaze
{
public:
	RenderedMaze(Maze);
	~RenderedMaze();

	void draw(RenderManager renderer);
private:
	Maze maze;
};

