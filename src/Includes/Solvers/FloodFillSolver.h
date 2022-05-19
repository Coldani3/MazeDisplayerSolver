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


    FloodFillSolver(Maze maze, std::shared_ptr<RenderManager> renderer);
    void solve();
    bool visitedCell(std::vector<int> coords);
    void clear();

private:
    std::queue<std::vector<int>> navQueue;
    std::vector<std::vector<int>> visited;
};

