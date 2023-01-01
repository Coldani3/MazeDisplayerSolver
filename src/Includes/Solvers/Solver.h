#include <Maze/Maze.h>
#include <Render/MainRenderManager.h>
#include <memory>

#include "../Maze/MazePathManager.h"

#pragma once
class Solver
{
public:
	Solver(std::shared_ptr<Maze> maze, std::shared_ptr<MazePathManager> pathManager);
	virtual void solve();
	float distanceBetween(const Coordinate<int>&, const Coordinate<int>&);
	virtual void clear();
	bool canAccessFrom(const Coordinate<int>& fromCoords, const Coordinate<int>& targetCoords);

	int stepsTaken = 0;
	bool success = false;

protected:
	std::shared_ptr<Maze> maze;
	std::shared_ptr<MazePathManager> pathManager;
};

