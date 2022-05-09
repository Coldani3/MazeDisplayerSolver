#pragma once
#include <Solvers/Solver.h>
class FungalSolver :
    public Solver
{
public:
    FungalSolver(Maze maze, std::shared_ptr<RenderManager> renderer, int maxSpreadPerNode);
    void solve();
    void spreadFrom(std::vector<int> from);
    std::vector<int> selectNewSpreadFromPoint(std::vector<int> previous);

private:
    std::vector<std::vector<int>> visited;
    int maxSpreadPerNode;
};

