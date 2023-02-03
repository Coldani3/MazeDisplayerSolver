#include "MazePathManager.h"

//We actually want to copy the path here.
void MazePathManager::setActivePath(MazePath path) noexcept {
	activePath = std::make_shared<MazePath>(path);
}

MazePath MazePathManager::getMazePath(std::string key, int id) const {
	return mazePaths.at(key)[id];
}
