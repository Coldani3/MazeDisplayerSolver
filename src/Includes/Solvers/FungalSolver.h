#pragma once
#include <Solvers/Solver.h>
class FungalSolver :
    public Solver
{
public:
    FungalSolver(std::shared_ptr<Maze> maze, std::shared_ptr<MazePathManager> pathManager, int maxSpreadPerNode);
    void solve();
    void spreadFrom(const Coordinate<int>& from);
    Coordinate<int> selectNewSpreadFromPoint(const Coordinate<int>& previous);

private:
    std::vector<std::vector<int>> visited;
    int maxSpreadPerNode;
};

