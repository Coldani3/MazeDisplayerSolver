#include "Maze/MazePathRenderProgress.h"

MazePathRenderProgress::MazePathRenderProgress(MazePath& path) : startingPath(path){
}

MazePathRenderProgress& MazePathRenderProgress::incrementProgress() {
	// TODO: insert return statement here
	progressedPath.markCellVisited(startingPath[progress++]);
	return *this;
}

MazePath MazePathRenderProgress::currentPath() {
	return progressedPath;
}
