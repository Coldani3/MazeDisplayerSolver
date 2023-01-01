#include <Solvers/FungalSolver.h>

FungalSolver::FungalSolver(std::shared_ptr<Maze> maze, std::shared_ptr<MazePathManager> pathManager, int maxSpreadPerNode) : Solver(maze, pathManager) {
	this->maxSpreadPerNode = maxSpreadPerNode;
}

void FungalSolver::solve() {
	spreadFrom(maze->mazeEntrance);
}

void FungalSolver::spreadFrom(const Coordinate<int>& from) {

}

Coordinate<int> FungalSolver::selectNewSpreadFromPoint(const Coordinate<int>& previous) {

	return std::vector<int>();
}