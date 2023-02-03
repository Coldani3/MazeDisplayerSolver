#include "MazeRenderInfo.h"

MazeRenderInfo::MazeRenderInfo(int startW) {
	this->wViewing = startW;
}

void MazeRenderInfo::changeWViewingForAnims(int w) {
	wViewing = w;
	beginWTransitionAnim(w);
}

//nextW is the W that it is going to be after the animation.
void MazeRenderInfo::beginWTransitionAnim(int nextW) {
	wChangeAnimStart = glfwGetTime();
	wTransitioning = true;
	this->nextW = nextW;
}

void MazeRenderInfo::finishWTransitionAnim() {
	lastW = wViewing;
	wViewing = nextW;
	wTransitioning = false;
	//reset animation start to 0 so there aren't any weird interactions with stopping one animation and immediately going into the next
	wChangeAnimStart = 0;
}