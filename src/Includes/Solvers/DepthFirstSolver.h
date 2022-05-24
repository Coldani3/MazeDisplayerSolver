#pragma once
#include <Solvers/Solver.h>
#include <stack>
#include <vector>

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

    DepthFirstSolver(std::shared_ptr<Maze> maze, std::shared_ptr<MainRenderManager> renderer);
    void solve();
    bool canGoAnywhereFrom(std::vector<int> from);
    bool visitedCell(std::vector<int> cell);
    void clear();
    std::vector<int> pickNextCellFrom(std::vector<int> from);

private:
    std::stack<std::vector<int>> navStack;
    std::vector<std::vector<int>> visited;
};

