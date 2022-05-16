#include <Maze/Maze.h>
#include <Render/RenderManager.h>

#pragma once
class MazeRenderer
{
public:
	MazeRenderer(Maze);
	~MazeRenderer();

	void render();
	void setup();
	void setMazeCenterProgram(int program);
	void setMazePathProgram(int program);
private:
	Maze maze;
	int mazeCenterProgram;
	int mazePathProgram;
};

