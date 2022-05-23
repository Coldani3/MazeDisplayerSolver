#include <Solvers/FungalSolver.h>

FungalSolver::FungalSolver(Maze maze, std::shared_ptr<MainRenderManager> renderer, int maxSpreadPerNode) : Solver(maze, renderer) {
	this->maxSpreadPerNode = maxSpreadPerNode;
}

void FungalSolver::solve() {
	spreadFrom(maze.mazeEntrance);
}

void FungalSolver::spreadFrom(std::vector<int> from) {

}

std::vector<int> FungalSolver::selectNewSpreadFromPoint(std::vector<int> previous) {

	return std::vector<int>();
}