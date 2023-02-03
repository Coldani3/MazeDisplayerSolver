#include "MazePathRenderProgress.h"

MazePathRenderProgress::MazePathRenderProgress(MazePath& path) : startingPath(path){
}

MazePathRenderProgress& MazePathRenderProgress::incrementProgress() {
	// TODO: insert return statement here
	progressedPath.markCellVisited(startingPath[progress++]);
	return *this;
}

MazePath MazePathRenderProgress::currentPath() const {
	MazePath pathCopy = progressedPath;
	return pathCopy;
}

size_t MazePathRenderProgress::size() const {
	return progressedPath.pathSize();
}
