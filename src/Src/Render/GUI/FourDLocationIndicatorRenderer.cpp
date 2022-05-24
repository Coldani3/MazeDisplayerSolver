#include <Render/GUI/FourDLocationIndicatorRenderer.h>

FourDLocationIndicatorRenderer::FourDLocationIndicatorRenderer(Maze maze) {
	this->maze = maze;
}

FourDLocationIndicatorRenderer::~FourDLocationIndicatorRenderer() {
	cleanup();
}

void FourDLocationIndicatorRenderer::setup() {
	std::cout << "[FourDLocationManager] Setting up shaders..." << std::endl;
	unsigned int indicatorVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(indicatorVertexShader, 1, &fourDIndicatorVertexShader, NULL);
	glCompileShader(indicatorVertexShader);
	checkShaderCompileSuccess(indicatorVertexShader);

	unsigned int indicatorFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(indicatorFragmentShader, 1, &fourDIndicatorFragmentShader, NULL);
	glCompileShader(indicatorFragmentShader);
	checkShaderCompileSuccess(indicatorFragmentShader);
	std::cout << "[FourDLocationManager] Done." << std::endl;

	std::cout << "[FourDLocationManager] Setting up programs..." << std::endl;
	indicatorProgram = glCreateProgram();
	glAttachShader(indicatorProgram, indicatorVertexShader);
	glAttachShader(indicatorProgram, indicatorFragmentShader);
	glLinkProgram(indicatorProgram);
	checkProgramCompileSuccess(indicatorProgram);
	std::cout << "[FourDLocationManager] Done." << std::endl;

	std::cout << "[FourDLocationManager] Cleaning up shaders..." << std::endl;
	glDeleteShader(indicatorVertexShader);
	glDeleteShader(indicatorFragmentShader);
	std::cout << "[FourDLocationManager] Done." << std::endl;

	std::cout << "[FourDLocationManager] Genning buffers..." << std::endl;
	glGenBuffers(1, &squareVBO);
	glGenBuffers(1, &squareEBO);
	glGenVertexArrays(1, &squareVAO);

	glBindVertexArray(squareVAO);
	glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(guiSquare), &guiSquare, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(guiSquareIndices), &guiSquareIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	std::cout << "[FourDLocationManager] Done." << std::endl;
}

void FourDLocationIndicatorRenderer::render() {
	//glDisable(GL_CULL_FACE);

	glUseProgram(indicatorProgram);

	for (int i = maze.hyperDepth; i >= 0; i--) {
		if (i == wViewing) {
			glUniform3fv(glGetUniformLocation(indicatorProgram, "squareColour"), 1, glm::value_ptr(inColour));
		} else {
			glUniform3fv(glGetUniformLocation(indicatorProgram, "squareColour"), 1, glm::value_ptr(notInColour));
		}

		//glUniformMatrix4fv "model", "projection"

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	//glEnable(GL_CULL_FACE);
}

void FourDLocationIndicatorRenderer::cleanup() {
	glDeleteProgram(indicatorProgram);
}