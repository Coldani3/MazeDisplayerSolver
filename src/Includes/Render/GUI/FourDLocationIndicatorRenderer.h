#pragma once

#include <Render/Renderer.h>
#include <Maze/Maze.h>

class FourDLocationIndicatorRenderer : public Renderer {
private:
	const float guiSquare[18] = {
		0.95f, 0.95f, 0.0f,
		0.90f, 0.95f, 0.0f,
		0.90f, 0.90f, 0.0f,
		0.95f,  0.90f, 0.0f
	};

	const float guiSquareIndices[6] = {
		0, 1, 2,
		0, 3, 2
	};

	glm::vec3 notInColour = glm::vec3(0.54f, 0.54f, 0.54f);
	//90, 237, 105 - greenish
	glm::vec3 inColour = glm::vec3(0.352f, 0.929f, 0.411f);

	unsigned int squareEBO;
	unsigned int squareVBO;
	unsigned int squareVAO;

	unsigned int indicatorProgram;

private:
	Maze maze;

	int wViewing;

public:
	FourDLocationIndicatorRenderer(Maze maze);
	~FourDLocationIndicatorRenderer();
	void setup();
	void render();
	void cleanup();

	void setWViewing(int);
};

