#include <string>

#pragma once
class Maze
{
public:
	Maze();
	~Maze();
	void loadFromFile(std::string fileName);
private:
	//i'm not bothering with higher than 4D here, bytes are fine
	unsigned char data[];
};

