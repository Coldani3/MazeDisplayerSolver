#pragma once
#include <Solvers/Solver.h>
#include <stack>
#include <vector>

class DepthFirstSolver :
    public Solver
{
public:
    DepthFirstSolver(Maze maze, std::shared_ptr<RenderManager> renderer);
    void solve();
    bool canGoAnywhereFrom(std::vector<int> from);
    std::vector<int> pickNextCellFrom(std::vector<int> from);

private:
    std::stack<std::vector<int>> navStack;
    std::vector<std::vector<int>> visited;
};

