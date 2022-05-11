#include <Solvers/DepthFirstSolver.h>
#include <algorithm>

std::vector<std::vector<int>> touchingSides = { 
	{1, 0, 0, 0}, 
	{0, 1, 0, 0},
	{0, 0, 1, 0},
	{0, 0, 0, 1},
	{-1, 0, 0, 0},
	{0, -1, 0, 0},
	{0, 0, -1, 0},
	{0, 0, 0, -1} 
};

DepthFirstSolver::DepthFirstSolver(Maze maze, std::shared_ptr<RenderManager> renderer) : Solver(maze, renderer) {
	visited = std::vector<std::vector<int>>();
	navStack = std::stack<std::vector<int>>();
}

void DepthFirstSolver::solve() {
	navStack.push(maze.mazeEntrance);
	visited.push_back(maze.mazeEntrance);
	renderer->markCellVisited(maze.mazeEntrance);

	std::cout << "[Depth First] Begin loop..." << std::endl;

	while (!navStack.empty()) {
		if (canGoAnywhereFrom(navStack.top())) {
			std::vector<int> next = pickNextCellFrom(navStack.top());

			visited.push_back(next);
			renderer->markCellVisited(next);

			navStack.push(next);

			++stepsTaken;

			if (next == maze.mazeExit) {
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

std::vector<int> DepthFirstSolver::pickNextCellFrom(std::vector<int> from) {
	//prefer positive coordinates first

	for (int i = 0; i < touchingSides.size(); i++) {
		std::vector<int> trying = addCoords(touchingSides[i], from);

		/*std::cout << "picking: {" << trying.size() << "} ";
		std::cout << trying[0];

		if (trying.size() > 1) {
			std::cout << ", " << trying[1] << ", " << trying[2] << ", " << trying[3] << std::endl;
		} else {
			std::cout << std::endl;
		}*/

		if (maze.inBounds(trying) && !visitedCell(trying) && canAccessFrom(from, trying)) {
			return trying;
		}
	}

	return { -1 };
}

bool DepthFirstSolver::canGoAnywhereFrom(std::vector<int> from) {
	return pickNextCellFrom(from) != std::vector<int> {-1};

	//return false;
}

bool DepthFirstSolver::visitedCell(std::vector<int> cell) {
	return std::find(visited.begin(), visited.end(), cell) != visited.end();
}

void DepthFirstSolver::clear() {
	visited.clear();
}
