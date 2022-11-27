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
	//stored mostly for the animations
	int lastW = 0;
	int nextW = 0;
	double wChangeAnimStart = 0;
	//how long it takes the 4D maze transition animation to complete in seconds. Usually used with wChangeAnimStart.
	double mazeTransitionAnimationSpeed = 0.5;
	bool wTransitioning = false;

	MazeRenderInfo(int startW);

	void changeWViewingForAnims(int w);
	void beginWTransitionAnim(int nextW);
	void finishWTransitionAnim();
};

