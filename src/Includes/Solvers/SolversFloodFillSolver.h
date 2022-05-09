#pragma once
#include <Solvers/Solver.h>

class FloodFillSolver :
    public Solver
{
public:
    FloodFillSolver(Maze maze, std::shared_ptr<RenderManager> renderer);
    void solve();
};

