#include <Render/GUI/FourDLocationIndicatorRenderer.h>

FourDLocationIndicatorRenderer::FourDLocationIndicatorRenderer(std::shared_ptr<TwoDCamera> camera, std::shared_ptr<Maze> maze) : GUIRenderer(camera) {
	this->maze = maze;
}

FourDLocationIndicatorRenderer::~FourDLocationIndicatorRenderer() {
	cleanup();
}

void FourDLocationIndicatorRenderer::setup() {
	setupShaders();

	std::cout << "[FourDLocationManager] Genning buffers..." << '\n';

	squareVBO
		.addConfiguration(VBOConfiguration(3))
		.loadData(squareVAO, guiSquare, GL_STATIC_DRAW)
		.loadEBOData(squareEBO, guiSquareIndices, GL_STATIC_DRAW)
		.initialise();

	std::cout << "[FourDLocationManager] Done." << std::endl;
}

void FourDLocationIndicatorRenderer::setupShaders() {
	std::cout << "[FourDLocationManager] Setting up shaders..." << '\n';
	indicatorProgram
		.loadVertexShader(fourDIndicatorVertexShader)
		.loadFragmentShader(fourDIndicatorFragmentShader)
		.createProgram();
}

void FourDLocationIndicatorRenderer::render(std::shared_ptr<MazeRenderInfo> mazeRenderInfo) {
	indicatorProgram.use();
	squareVAO.bind();

	float hyperDepth = static_cast<float>(maze->hyperDepth);
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), squareSize);
	glm::mat4 initialTranslate = glm::translate(glm::mat4(1.0f), position);

	float xTransPerSlice = -200.0f / hyperDepth;
	float yTransPerSlice = 80.0f / hyperDepth;

	//draw layer 0 on top and at the bottom left of the others by starting with the last layer
	for (int i = maze->hyperDepth - 1; i >= 0; i--) {
		if (i == mazeRenderInfo->wViewing) {
			indicatorProgram.uniform("squareColour", glm::vec4(inColour, 0.9f));
		} else {
			indicatorProgram.uniform("squareColour", glm::vec4(notInColour, 0.6f));
		}

		//remember it is SCREEN coordinates, ie. 1 to -1 both axes.
		//
		float translateScale = hyperDepth - i;

		//TODO: scale translation by how many slices there are (max 200pxX80px?).

		glm::mat4 model = getSliceTranslate(xTransPerSlice, yTransPerSlice, translateScale, initialTranslate) * scale;

		indicatorProgram.uniform("model", model);
		indicatorProgram.uniform("projection", camera->getProjection());

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	VAO<float>::unbindVAOs();
}

void FourDLocationIndicatorRenderer::cleanup() {
	std::cout << "Cleaning up 4D indicator renderer..." << '\n';
	deleteProgramIfExists(indicatorProgram.getProgram(), "indicatorProgram");

	std::cout << "Done." << std::endl;
}

void FourDLocationIndicatorRenderer::updatePosition() {
	position = screenRelativeCoords(0.9f, -0.9f);
}

glm::mat4 FourDLocationIndicatorRenderer::getSliceTranslate(float xTransPerSlice, float yTransPerSlice, float translateScale, const glm::mat4& initialTranslate) {
	return glm::translate(glm::mat4(1.0f), glm::vec3(xTransPerSlice, yTransPerSlice, 0.0f) * translateScale) * initialTranslate;
}

std::shared_ptr<Camera> FourDLocationIndicatorRenderer::getCamera() const {
	return camera;
}

void FourDLocationIndicatorRenderer::getRenderPollInput(GLFWwindow* window, double delta, const InputManager& inputManager) {

}
