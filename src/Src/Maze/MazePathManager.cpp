#include "Maze/MazePathManager.h"

void MazePathManager::setActivePath(const MazePath& path) noexcept {
	activePath = std::make_shared<MazePath>(path);
}

MazePath MazePathManager::getMazePath(std::string key, int id) const {
	return mazePaths.at(key)[id];
}
