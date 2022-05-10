#include <Solvers/DepthFirstSolver.h>
#include <algorithm>



DepthFirstSolver::DepthFirstSolver(Maze maze, std::shared_ptr<RenderManager> renderer) : Solver(maze, renderer) {
	visited = std::vector<std::vector<int>>();
	navStack = std::stack<std::vector<int>>();
}

void DepthFirstSolver::solve() {
	navStack.push(maze.mazeEntrance);
	visited.push_back(maze.mazeEntrance);
	renderer->markCellVisited(maze.mazeEntrance);

	std::cout << "[Depth First] Begin loop..." << std::endl;

	while (navStack.size() > 0) {
		if (canGoAnywhereFrom(navStack.top())) {
			std::vector<int> next = pickNextCellFrom(navStack.top());

			visited.push_back(next);
			renderer->markCellVisited(next);

			navStack.push(next);

			++stepsTaken;

			if (next == maze.mazeEntrance) {
				std::cout << "[Depth First] Found exit!" << std::endl;
				success = true;
				break;
			}
		} else {
			//backtrack
			while (!canGoAnywhereFrom(navStack.top()) && !navStack.empty()) {
				navStack.pop();
			}
		}
	}
}

std::vector<int> DepthFirstSolver::pickNextCellFrom(std::vector<int> from) {
	//prefer positive coordinates first

	for (int x = 1; x >= -1; x -= 2) {
		for (int y = 1; y >= -1; y -= 2) {
			for (int z = 1; z >= -1; z -= 2) {
				for (int w = 1; w >= -1; w -= 2) {
					std::vector<int> trying = { from[0] + x, from[1] + y, from[2] + z, from[3] + w };

					if (maze.inBounds(trying) && !visitedCell(trying)) {
						return trying;
					}
				}
			}
		}
	}

	return std::vector<int>(-1);
}

bool DepthFirstSolver::canGoAnywhereFrom(std::vector<int> from) {
	for (int x = -1; x <= 1; x += 2) {
		for (int y = -1; y <= 1; y += 2) {
			for (int z = -1; z <= 1; z += 2) {
				for (int w = -1; w <= 1; w += 2) {
					std::vector<int> coords = { from[0] + x, from[1] + y, from[2] + z, from[3] + w };

					//if coords in visitedCell
					if (maze.inBounds(coords) && !visitedCell(coords)) {
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool DepthFirstSolver::visitedCell(std::vector<int> cell) {
	return std::find(visited.begin(), visited.end(), cell) != visited.end();
}