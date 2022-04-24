#include <Maze/Maze.h>

#pragma once
class Solver
{
public:
	Solver(Maze maze);
	void solve();

protected:
	Maze maze;
};

