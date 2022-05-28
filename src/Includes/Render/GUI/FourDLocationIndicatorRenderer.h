#pragma once

#include <Render/Renderer.h>
#include <Maze/Maze.h>
#include <Render/PerspectiveCamera.h>
#include <Render/GUI/TwoDCamera.h>

class FourDLocationIndicatorRenderer : public Renderer {
private:
	const float guiSquare[12] = {
		0.95f, 0.95f, 0.0f, // top right
		0.80f, 0.95f, 0.0f, // top left
		0.80f, 0.80f, 0.0f, // bottom left
		0.95f, 0.80f, 0.0f // bottom right
	};

	const unsigned int guiSquareIndices[6] = {
		0, 1, 2,
		2, 3, 0
	};

	glm::vec3 notInColour = glm::vec3(0.54f, 0.54f, 0.54f);
	//90, 237, 105 - greenish
	glm::vec3 inColour = glm::vec3(0.352f, 0.929f, 0.411f);

	unsigned int squareEBO;
	unsigned int squareVBO;
	unsigned int squareVAO;

	unsigned int indicatorProgram;

private:
	std::shared_ptr<Maze> maze;

	int wViewing = 0;

public:
	FourDLocationIndicatorRenderer(TwoDCamera camera, std::shared_ptr<Maze> maze);
	~FourDLocationIndicatorRenderer();
	void setup();
	void render();
	void cleanup();

	void setWViewing(int);
};

