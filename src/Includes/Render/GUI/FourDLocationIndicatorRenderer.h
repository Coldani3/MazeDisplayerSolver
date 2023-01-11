#pragma once

#include <Render/GUI/GUIRenderer.h>
#include <Maze/Maze.h>
#include <Render/PerspectiveCamera.h>
#include <Render/GUI/TwoDCamera.h>
#include "../ShaderProgram.h"
#include "../Buffers/VBO.h"
#include "../Buffers/VBOConfiguration.h"
#include "../Buffers/VAO.h"
#include "../Buffers/EBO.h"

class FourDLocationIndicatorRenderer : public GUIRenderer {
private:
	const std::vector<float> guiSquare = {
		1.0f, 1.0f, 0.0f, // top right
		0.0f, 1.0f, 0.0f, // top left
		0.0f, 0.0f, 0.0f, // bottom left
		1.0f, 0.0f, 0.0f // bottom right
	};

	const std::vector<int> guiSquareIndices = {
		0, 1, 2,
		2, 3, 0
	};

	glm::vec3 notInColour = glm::vec3(0.54f, 0.54f, 0.54f);
	//90, 237, 105 - greenish
	glm::vec3 inColour = glm::vec3(0.352f, 0.929f, 0.411f);
	glm::vec3 position = screenRelativeCoords(0.9f, -0.9f);
	glm::vec3 squareSize = glm::vec3(100.0f, 100.0f, 1.0f);

	EBO squareEBO;
	VBOf squareVBO;
	VAO<float> squareVAO;

	//unsigned int indicatorProgram;
	ShaderProgram indicatorProgram = ShaderProgram("4D Location Indicator");

private:
	std::shared_ptr<Maze> maze = nullptr;

public:
	FourDLocationIndicatorRenderer(std::shared_ptr<TwoDCamera> camera, std::shared_ptr<Maze> maze);
	~FourDLocationIndicatorRenderer();
	void setup();
	void setupShaders();
	void render(std::shared_ptr<MazeRenderInfo> mazeRendererInfo);
	void cleanup();
	void updatePosition();
	glm::mat4 getSliceTranslate(float xTransPerSlice, float yTransPerSlice, float translateScale, const glm::mat4& initialTranslate);

	virtual std::shared_ptr<Camera> getCamera() const override;

	void getRenderPollInput(GLFWwindow* window, double delta, const InputManager& inputManager) override;
};

