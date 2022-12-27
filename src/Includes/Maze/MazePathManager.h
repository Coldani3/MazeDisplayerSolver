#include <map>
#include <vector>
#include <string>
#include <memory>

#include "MazePath.h"

#pragma once
class MazePathManager {
	//Keyed by the AI solver.
	std::map<std::string, std::vector<MazePath>> mazePaths;


public:
	std::shared_ptr<MazePath> activePath;

	void setActivePath(MazePath path) noexcept;
	MazePath getMazePath(std::string key, int id) const;
	void putMazePath(std::string key, const MazePath& path) noexcept;
};

