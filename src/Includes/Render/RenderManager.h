#include <glfw3/glfw3.h>

#include <glm/glm.hpp>

#include <memory>
#include <Render/Camera.h>

#pragma once
class RenderManager
{
public:
	RenderManager(int width, int height);
	~RenderManager();
	void setup();
	void draw();
	void framebufferSizeCallback(GLFWwindow* window, int width, int height);
	GLFWwindow* getWindow();
	int getWidth();
	int getHeight();
	void drawMazeCellCenter(int mazeX, int mazeY, int mazeZ = 0, int mazeW = 0);
	void drawMazeCellPaths(unsigned char mazeCellData, int mazeX, int mazeY, int mazeZ = 0, int mazeW = 0);
	void setWViewing(int w);
	glm::mat4 mazeCellPathTransform(float rotateAngleX, float rotateAngleY, float translateX, float translateY, float translateZ);
	glm::mat4 getViewMatrixFromCamera();

	glm::mat4 projection;
	std::unique_ptr<Camera> camera;
private:
	GLFWwindow* window;
	int width;
	int height;
	int defaultWidth;
	int defaultHeight;
	int currentW = 0;
};

