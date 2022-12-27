#include "MazePath.h"

#pragma once
class MazePathRenderProgress
{
	MazePath& startingPath;
	MazePath progressedPath;
	int progress = 0;
public:
	MazePathRenderProgress(MazePath& path);
	MazePathRenderProgress& incrementProgress();
	MazePath currentPath();
};

