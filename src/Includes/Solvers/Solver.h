#include <Maze/Maze.h>
#include <Render/RenderManager.h>
#include <memory>

#pragma once
class Solver
{
public:
	Solver(Maze maze, std::shared_ptr<RenderManager> renderer);
	virtual void solve();
	float distanceBetween(std::vector<int>, std::vector<int>);
	std::vector<int> addCoords(std::vector<int> coords1, std::vector<int> coords2) const;
	std::vector<float> floatify(std::vector<int> vec);
	std::vector<int> intify(std::vector<float> vec);
	std::vector<int> minusCoords(std::vector<int> coords1, std::vector<int> coords2);
	virtual void clear();
	bool canAccessFrom(std::vector<int> fromCoords, std::vector<int> targetCoords);

	int stepsTaken = 0;
	bool success = false;

protected:
	Maze maze;
	std::shared_ptr<RenderManager> renderer;
};

