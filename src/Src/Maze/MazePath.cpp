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

MazePath::MazePath(const MazePath& path) noexcept {
    mazeWidth = path.mazeWidth;
    mazeHeight = path.mazeHeight;
    mazeDepth = path.mazeDepth;
    mazeHyperDepth = path.mazeHyperDepth;

    visited = path.visited;
}

MazePath::MazePath(MazePath&& path) noexcept {
    mazeWidth = std::exchange(path.mazeWidth, 0);
    mazeHeight = std::exchange(path.mazeHeight, 0);
    mazeDepth = std::exchange(path.mazeDepth, 0);
    mazeHyperDepth = std::exchange(path.mazeHyperDepth, 0);

    visited = std::move(path.visited);
}

MazePath::~MazePath() {
}

bool MazePath::visitedCell(const Coordinate<int>& coords) const {
    return visited[(mazeHeight * mazeWidth * mazeDepth * coords.w()) + (mazeHeight * mazeWidth * coords.z()) + (mazeHeight * coords.y()) + coords.x()];
}

void MazePath::markCellVisited(const Coordinate<int>& coords) {
    if (!visitedCell(coords)) {
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
