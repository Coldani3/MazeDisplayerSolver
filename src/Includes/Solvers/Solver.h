#include <Maze/Maze.h>

#pragma once
class Solver
{
public:
	Solver(Maze maze);
	void solve();
	float distanceBetween(std::vector<int>, std::vector<int>);
	std::vector<int> addCoords(std::vector<int> coords1, std::vector<int> coords2);
	std::vector<float> floatify(std::vector<int> vec);
	std::vector<int> intify(std::vector<float> vec);
	std::vector<int> minusCoords(std::vector<int> coords1, std::vector<int> coords2);

protected:
	Maze maze;
};

