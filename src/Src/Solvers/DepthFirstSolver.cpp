#include <Solvers/DepthFirstSolver.h>
#include <algorithm>



DepthFirstSolver::DepthFirstSolver(Maze maze, std::shared_ptr<RenderManager> renderer) : Solver(maze, renderer) {

}

void DepthFirstSolver::solve() {
	navStack.push(maze.mazeEntrance);

	while (navStack.size() > 0) {
		if (canGoAnywhereFrom(navStack.top())) {
			std::vector<int> next = pickNextCellFrom(navStack.top());

			renderer->markCellVisited(next);

			navStack.push(next);
		} else {
			//backtrack
		}
	}
}

std::vector<int> DepthFirstSolver::pickNextCellFrom(std::vector<int> from) {
	//prefer positive coordinates first

	for (int x = 1; x >= -1; x -= 2) {
		for (int y = 1; y >= -1; y -= 2) {
			for (int z = 1; z >= -1; z -= 2) {
				for (int w = 1; w >= -1; w -= 2) {
				}
			}
		}
	}

	return std::vector<int>();
}

bool DepthFirstSolver::canGoAnywhereFrom(std::vector<int> from) {
	for (int x = -1; x <= 1; x += 2) {
		for (int y = -1; y <= 1; y += 2) {
			for (int z = -1; z <= 1; z += 2) {
				for (int w = -1; w <= 1; w += 2) {
					std::vector<int> coords = { x, y, z, w };
					//if coords in visited
					if (std::find(visited.begin(), visited.end(), coords) != visited.end()) {
						return true;
					}
				}
			}
		}
	}

	return false;
}