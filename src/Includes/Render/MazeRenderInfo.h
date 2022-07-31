#include <memory>
#include <Maze/Maze.h>
#include <glfw3/glfw3.h>

#pragma once
/*
* Object that stores information about the rendering of a specific maze. Used to synchronise what w each Renderer
* is viewing, as well as eventually other settings.
* TODO: Could this be a struct?
*/
class MazeRenderInfo {
public:
	int wViewing = 0;
	int nextW = 0;
	double lastWChange = 0;
	//how long it takes the 4D maze transition animation to complete in seconds. Usually used with lastWChange.
	double mazeTransitionAnimationSpeed = 0.5;
	bool wTransitioning = false;

	MazeRenderInfo(int startW);

	void changeWViewingForAnims(int w);
	void beginWTransitionAnim(int nextW);
	void finishWTransitionAnim();
};

