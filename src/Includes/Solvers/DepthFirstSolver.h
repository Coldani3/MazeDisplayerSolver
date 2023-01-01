#pragma once
#include <Solvers/Solver.h>
#include <stack>
#include <vector>
#include "../Maths/Consts.h"

class DepthFirstSolver :
    public Solver
{
public:
    const std::vector<std::vector<int>> touchingSides = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1},
        {-1, 0, 0, 0},
        {0, -1, 0, 0},
        {0, 0, -1, 0},
        {0, 0, 0, -1}
    };

    DepthFirstSolver(std::shared_ptr<Maze> maze, std::shared_ptr<MazePathManager> pathManager);
    void solve();
    bool canGoAnywhereFrom(const Coordinate<int>& from);
    bool visitedCell(const Coordinate<int>& cell);
    void clear();
    Coordinate<int> pickNextCellFrom(const Coordinate<int>& from);

private:
    std::stack<Coordinate<int>> navStack;
    std::vector<Coordinate<int>> visited;
};

