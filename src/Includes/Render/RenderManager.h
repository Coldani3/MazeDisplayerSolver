#include <glfw3/glfw3.h>

#pragma once
class RenderManager
{
public:
	RenderManager();
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
private:
	GLFWwindow* window;
	int width;
	int height;
	int defaultWidth;
	int defaultHeight;
};

