#pragma once

#include <vector>
#include "../Math/Coordinate.h"

class MazePath {
public:
	MazePath();
	MazePath(int mazeWidth, int mazeHeight, int mazeDepth, int mazeHyperDepth);
	MazePath(const MazePath& path) noexcept;
	MazePath(MazePath&& path) noexcept;
	~MazePath();
	bool visitedCell(const Coordinate<int>& coords) const;
	void markCellVisited(const Coordinate<int>& coords);
	void clearVisitedCells();
	size_t pathSize() const;

	Coordinate<int> operator[](int index);

private:
	std::vector<bool> visited;
	std::vector<Coordinate<int>> visitedPath;
	int mazeHeight = 0;
	int mazeWidth = 0;
	int mazeDepth = 0;
	int mazeHyperDepth = 0;
};

