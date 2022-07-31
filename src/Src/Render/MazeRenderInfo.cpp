#include <Render/MazeRenderInfo.h>

MazeRenderInfo::MazeRenderInfo(int startW) {
	this->wViewing = startW;
}

void MazeRenderInfo::changeWViewingForAnims(int w) {
	wViewing = w;
	beginWTransitionAnim(w);
}

//nextW is the W that it is going to be after the animation.
void MazeRenderInfo::beginWTransitionAnim(int nextW) {
	lastWChange = glfwGetTime();
	wTransitioning = true;
	this->nextW = nextW;
}

void MazeRenderInfo::finishWTransitionAnim() {
	wViewing = nextW;
	wTransitioning = false;
}