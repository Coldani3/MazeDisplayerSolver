#include <Render/GUI/FourDLocationIndicatorRenderer.h>

FourDLocationIndicatorRenderer::FourDLocationIndicatorRenderer(std::shared_ptr<TwoDCamera> camera, std::shared_ptr<Maze> maze) : GUIRenderer(camera) {
	this->maze = maze;
}

FourDLocationIndicatorRenderer::~FourDLocationIndicatorRenderer() {
	cleanup();
}

void FourDLocationIndicatorRenderer::setup() {
	std::cout << "[FourDLocationManager] Setting up shaders..." << '\n';
	unsigned int indicatorVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(indicatorVertexShader, 1, &fourDIndicatorVertexShader, NULL);
	glCompileShader(indicatorVertexShader);
	checkShaderCompileSuccess(indicatorVertexShader);

	unsigned int indicatorFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(indicatorFragmentShader, 1, &fourDIndicatorFragmentShader, NULL);
	glCompileShader(indicatorFragmentShader);
	checkShaderCompileSuccess(indicatorFragmentShader);
	std::cout << "[FourDLocationManager] Done." << '\n';

	std::cout << "[FourDLocationManager] Setting up programs..." << '\n';
	indicatorProgram = glCreateProgram();
	glAttachShader(indicatorProgram, indicatorVertexShader);
	glAttachShader(indicatorProgram, indicatorFragmentShader);
	glLinkProgram(indicatorProgram);
	checkProgramCompileSuccess(indicatorProgram);
	std::cout << "[FourDLocationManager] Done." << '\n';

	std::cout << "[FourDLocationManager] Cleaning up shaders..." << '\n';
	glDeleteShader(indicatorVertexShader);
	glDeleteShader(indicatorFragmentShader);
	std::cout << "[FourDLocationManager] Done." << '\n';

	std::cout << "[FourDLocationManager] Genning buffers..." << '\n';
	glGenBuffers(1, &squareVBO);
	glGenVertexArrays(1, &squareVAO);
	glGenBuffers(1, &squareEBO);

	glBindVertexArray(squareVAO);
	glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(guiSquare), &guiSquare, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(guiSquareIndices), &guiSquareIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	std::cout << "[FourDLocationManager] Done." << std::endl;
}

void FourDLocationIndicatorRenderer::render(std::shared_ptr<MazeRenderInfo> mazeRenderInfo) {
	glUseProgram(indicatorProgram);
	glBindVertexArray(squareVAO);

	float hyperDepth = (float)maze->hyperDepth;
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), squareSize);
	glm::mat4 initialTranslate = glm::translate(glm::mat4(1.0f), position);

	float xTransPerSlice = -200.0f / hyperDepth;
	float yTransPerSlice = 80.0f / hyperDepth;

	//draw layer 0 on top and at the bottom left of the others by starting with the last layer
	for (int i = maze->hyperDepth - 1; i >= 0; i--) {
		if (i == mazeRenderInfo->wViewing) {
			glUniform4fv(glGetUniformLocation(indicatorProgram, "squareColour"), 1, glm::value_ptr(glm::vec4(inColour, 0.9f)));
		} else {
			glUniform4fv(glGetUniformLocation(indicatorProgram, "squareColour"), 1, glm::value_ptr(glm::vec4(notInColour, 0.6f)));
		}

		//remember it is SCREEN coordinates, ie. 1 to -1 both axes.
		//
		float translateScale = hyperDepth - i;

		//glm::translate(glm::mat4(1.0f), glm::vec3(-0.01f, -0.005f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(3000.0f, 3000.0f, 1.0f)) * translateScale;

		//TODO: scale translation by how many slices there are (max 200pxX80px?).
		glm::mat4 translate = (glm::translate(glm::mat4(1.0f), glm::vec3(xTransPerSlice, yTransPerSlice, 0.0f) * translateScale)) * initialTranslate;

		glm::mat4 model = translate * scale;


		glUniformMatrix4fv(glGetUniformLocation(indicatorProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(glGetUniformLocation(indicatorProgram, "projection"), 1, GL_FALSE, glm::value_ptr(camera->getProjection()));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	glBindVertexArray(0);
}

void FourDLocationIndicatorRenderer::cleanup() {
	std::cout << "Cleaning up 4D indicator renderer..." << '\n';
	deleteProgramIfExists(indicatorProgram, "indicatorProgram");

	std::cout << "Done." << std::endl;
}

void FourDLocationIndicatorRenderer::updatePosition() {
	position = screenRelativeCoords(0.9f, -0.9f);
}

std::shared_ptr<Camera> FourDLocationIndicatorRenderer::getCamera() {
	return camera;
}

void FourDLocationIndicatorRenderer::getRenderPollInput(GLFWwindow* window, double delta) {

}
