#include <Solvers/FungalSolver.h>

FungalSolver::FungalSolver(std::shared_ptr<Maze> maze, std::shared_ptr<MazePathManager> pathManager, int maxSpreadPerNode) : Solver(maze, pathManager) {
	this->maxSpreadPerNode = maxSpreadPerNode;
}

void FungalSolver::solve() {
	spreadFrom(maze->mazeEntrance);
}

void FungalSolver::spreadFrom(std::vector<int> from) {

}

std::vector<int> FungalSolver::selectNewSpreadFromPoint(std::vector<int> previous) {

	return std::vector<int>();
}