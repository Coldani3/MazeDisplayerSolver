#include <string>
#include <iostream>

#pragma once
class Maze
{
public:
	Maze();
	Maze(int dimensions[]);
	~Maze();
	void loadFromFile(std::string fileName);
private:
	int width;
	int height;
	int depth;
	int hyperDepth;
	//i'm not bothering with higher than 4D here, bytes are fine
	std::vector mazeData;
};

