#pragma once

#include <vector>

class MazePath {
public:
	MazePath();
	MazePath(int mazeWidth, int mazeHeight, int mazeDepth, int mazeHyperDepth);
	~MazePath();
	bool visitedCell(std::vector<int> coords);
	void markCellVisited(std::vector<int> coords);
	void clearVisitedCells();
	int pathSize();

private:
	std::vector<bool> visited;
	std::vector<std::vector<int>> visitedPath;
	int mazeHeight = 0;
	int mazeWidth = 0;
	int mazeDepth = 0;
	int mazeHyperDepth = 0;
};

