#pragma once

#include <Render/GUI/GUIRenderer.h>
#include <Maze/Maze.h>
#include <Render/PerspectiveCamera.h>
#include <Render/GUI/TwoDCamera.h>

class FourDLocationIndicatorRenderer : public GUIRenderer {
private:
	const float guiSquare[12] = {
		1.0f, 1.0f, 0.0f, // top right
		0.0f, 1.0f, 0.0f, // top left
		0.0f, 0.0f, 0.0f, // bottom left
		1.0f, 0.0f, 0.0f // bottom right
	};

	const unsigned int guiSquareIndices[6] = {
		0, 1, 2,
		2, 3, 0
	};

	glm::vec3 notInColour = glm::vec3(0.54f, 0.54f, 0.54f);
	//90, 237, 105 - greenish
	glm::vec3 inColour = glm::vec3(0.352f, 0.929f, 0.411f);
	glm::vec3 position = screenRelativeCoords(0.8f, -0.8f);
	glm::vec3 squareSize = glm::vec3(100.0f, 100.0f, 1.0f);

	unsigned int squareEBO;
	unsigned int squareVBO;
	unsigned int squareVAO;

	unsigned int indicatorProgram;

private:
	std::shared_ptr<Maze> maze;

	int wViewing = 0;

public:
	FourDLocationIndicatorRenderer(std::shared_ptr<TwoDCamera> camera, std::shared_ptr<Maze> maze);
	~FourDLocationIndicatorRenderer();
	void setup();
	void render();
	void cleanup();

	void setWViewing(int);
};

