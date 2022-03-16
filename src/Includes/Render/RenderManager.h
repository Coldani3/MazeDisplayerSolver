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
private:
	GLFWwindow* window;
	int width;
	int height;
	int defaultWidth;
	int defaultHeight;
};

