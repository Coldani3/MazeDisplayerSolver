#include <Solvers/FloodFillSolver.h>

std::vector<std::vector<int>> floodTouchingSides = {
	{1, 0, 0, 0},
	{-1, 0, 0, 0},
	{0, 1, 0, 0},
	{0, -1, 0, 0},
	{0, 0, 1, 0},
	{0, 0, -1, 0},
	{0, 0, 0, 1},
	{0, 0, 0, -1}
};

FloodFillSolver::FloodFillSolver(Maze maze, std::shared_ptr<RenderManager> renderer) : Solver(maze, renderer) {
	navQueue = std::queue<std::vector<int>>();
	visited = std::vector<std::vector<int>>();
}

void FloodFillSolver::solve() {
	navQueue.push(maze.mazeEntrance);

	while (!navQueue.empty()) {
		std::vector<int> top = navQueue.front();
		navQueue.pop();

		if (maze.mazeExit == top) {
			success = true;
			break;
		}

		renderer->markCellVisited(top);
		visited.push_back(top);

		for (int i = 0; i < floodTouchingSides.size(); i++) {
			std::vector<int> newCoords = addCoords(top, floodTouchingSides[i]);

			if (maze.inBounds(newCoords) && !visitedCell(newCoords) && canAccessFrom(top, newCoords)) {
				navQueue.push(addCoords(top, floodTouchingSides[i]));
			}
		}

		++stepsTaken;
	}
}

bool FloodFillSolver::visitedCell(std::vector<int> coords) {
	return std::find(visited.begin(), visited.end(), coords) != visited.end();
}

void FloodFillSolver::clear() {
	std::queue<std::vector<int>> empty;
	std::swap(navQueue, empty);
	visited.clear();
}
