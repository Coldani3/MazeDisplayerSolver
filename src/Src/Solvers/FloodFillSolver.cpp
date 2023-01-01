#include <Solvers/FloodFillSolver.h>

FloodFillSolver::FloodFillSolver(std::shared_ptr<Maze> maze, std::shared_ptr<MazePathManager> pathManager) : Solver(maze, pathManager) {
	navQueue = std::queue<Coordinate<int>>();
	visited = std::vector<Coordinate<int>>();
}

void FloodFillSolver::solve() {
	navQueue.push(maze->mazeEntrance);

	while (!navQueue.empty()) {
		Coordinate<int> top = navQueue.front();
		navQueue.pop();

		if (maze->mazeExit == top) {
			success = true;
			break;
		}

		pathManager->activePath->markCellVisited(top);
		visited.push_back(top);

		for (Coordinate<int> floodTouchingSide : floodTouchingSides) {
			Coordinate<int> newCoords = top + floodTouchingSide;

			if (maze->inBounds(newCoords) && 
				!visitedCell(newCoords) && 
				canAccessFrom(top, newCoords)) {
				navQueue.push(top + floodTouchingSide);
			}
		}

		++stepsTaken;
	}
}

bool FloodFillSolver::visitedCell(const Coordinate<int>& coords) {
	return std::find(visited.begin(), visited.end(), coords) != visited.end();
}

void FloodFillSolver::clear() {
	std::queue<Coordinate<int>> empty;
	std::swap(navQueue, empty);
	visited.clear();
}
