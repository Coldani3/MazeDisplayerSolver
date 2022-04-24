#pragma once
#include <Solvers/Solver.h>

class DepthFirstSolver :
    public Solver
{
public:
    DepthFirstSolver(Maze maze);
    void solve();
};

