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

bool MazePath::visitedCell(std::vector<int> coords) {
    return visited[(mazeHeight * mazeWidth * mazeDepth * coords[3]) + (mazeHeight * mazeWidth * coords[2]) + (mazeHeight * coords[1]) + coords[0]];
}

void MazePath::markCellVisited(std::vector<int> coords) {
    if (!(visitedCell(coords))) {
        visited[(mazeHeight * mazeWidth * mazeDepth * coords[3]) + (mazeHeight * mazeWidth * coords[2]) + (mazeHeight * coords[1]) + coords[0]] = true;
        visitedPath.push_back(coords);
    }
}

void MazePath::clearVisitedCells() {
    visited.clear();
    visited = std::vector<bool>();
    visited.resize(mazeWidth * mazeHeight * mazeDepth * mazeHyperDepth);
    visitedPath.clear();
}

int MazePath::pathSize() {
    return visited.size();
}
