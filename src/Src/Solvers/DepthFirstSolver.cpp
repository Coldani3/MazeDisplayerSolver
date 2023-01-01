#include <Solvers/DepthFirstSolver.h>
#include <algorithm>

DepthFirstSolver::DepthFirstSolver(std::shared_ptr<Maze> maze, std::shared_ptr<MazePathManager> pathManager) : Solver(maze, pathManager) {
	visited = std::vector<Coordinate<int>>();
	navStack = std::stack<Coordinate<int>>();
}

void DepthFirstSolver::solve() {
	navStack.push(maze->mazeEntrance);
	visited.push_back(maze->mazeEntrance);
	pathManager->activePath->markCellVisited(maze->mazeEntrance);

	std::cout << "[Depth First] Begin loop..." << std::endl;

	while (!navStack.empty()) {
		if (canGoAnywhereFrom(navStack.top())) {
			Coordinate<int> next = pickNextCellFrom(navStack.top());

			visited.push_back(next);
			pathManager->activePath->markCellVisited(next);

			navStack.push(next);

			++stepsTaken;

			if (next == maze->mazeExit) {
				std::cout << "[Depth First] Found exit!" << std::endl;
				success = true;
				break;
			}
		} else {
			//remember the teleporting rule so no steps taken increment
			//backtrack
			while (!navStack.empty() && !canGoAnywhereFrom(navStack.top())) {
				navStack.pop();
			}
		}
	}
}

Coordinate<int> DepthFirstSolver::pickNextCellFrom(const Coordinate<int>& from) {
	//prefer positive coordinates first

	for (Coordinate<int> touchingSide : Consts::TouchingSides) {
		Coordinate<int> trying = touchingSide + from;//addCoords(touchingSides[i], from);

		if (maze->inBounds(trying) && 
			!pathManager->activePath->visitedCell(trying) && 
			canAccessFrom(from, trying)) {
			return trying;
		}
	}

	return Coordinate<int>({ 0, 0, 0, -1 });
}

bool DepthFirstSolver::canGoAnywhereFrom(const Coordinate<int>& from) {
	return pickNextCellFrom(from).w() != -1;
}

bool DepthFirstSolver::visitedCell(const Coordinate<int>& cell) {
	return std::find(visited.begin(), visited.end(), cell) != visited.end();
}

void DepthFirstSolver::clear() {
	visited.clear();
}