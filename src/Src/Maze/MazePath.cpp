#include <Maze/MazePath.h>

MazePath::MazePath() {
}

MazePath::MazePath(int mazeWidth, int mazeHeight, int mazeDepth, int mazeHyperDepth) {
    this->mazeWidth = mazeWidth;
    this->mazeHeight = mazeHeight;
    this->mazeDepth = mazeDepth;
    this->mazeHyperDepth = mazeHyperDepth;
    visited = std::vector<bool>();
    visited.resize(mazeWidth * mazeHeight * mazeDepth * mazeHyperDepth);
}

MazePath::~MazePath() {
}

bool MazePath::visitedCell(const Coordinate<int>& coords) const {
    return visited[(mazeHeight * mazeWidth * mazeDepth * coords.w()) + (mazeHeight * mazeWidth * coords.z()) + (mazeHeight * coords.y()) + coords.x()];
}

void MazePath::markCellVisited(const Coordinate<int>& coords) {
    if (!(visitedCell(coords))) {
        visited[(mazeHeight * mazeWidth * mazeDepth * coords.w()) + (mazeHeight * mazeWidth * coords.z()) + (mazeHeight * coords.y()) + coords.x()] = true;
        visitedPath.push_back(coords);
    }
}

void MazePath::clearVisitedCells() {
    visited.clear();
    visited = std::vector<bool>();
    visited.resize(mazeWidth * mazeHeight * mazeDepth * mazeHyperDepth);
    visitedPath.clear();
}

size_t MazePath::pathSize() const {
    return visitedPath.size();
}

Coordinate<int> MazePath::operator[](int index) {
    return visitedPath[index];
}
