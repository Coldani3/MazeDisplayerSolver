#pragma once
#include <Solvers/Solver.h>
#include <queue>
#include <vector>

class FloodFillSolver :
    public Solver
{
public:
	const std::vector<std::vector<int>> floodTouchingSides = {
		{1, 0, 0, 0},
		{-1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, -1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, -1, 0},
		{0, 0, 0, 1},
		{0, 0, 0, -1}
	};


    FloodFillSolver(std::shared_ptr<Maze> maze, std::shared_ptr<MazePathManager> pathManager);
    void solve();
    bool visitedCell(const Coordinate<int>& coords);
    void clear();

private:
    std::queue<Coordinate<int>> navQueue;
    std::vector<Coordinate<int>> visited;
};

