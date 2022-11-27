#ifndef __gl_h
#include <glad/glad.h>
#endif

#include <glfw3/glfw3.h>
#include <string>

#pragma once
class Window {
	GLFWwindow* window;
	int width, height, defaultWidth, defaultHeight;
public:
	Window(int width, int height, std::string title);

	void setWindowTitle(std::string title);
	bool shouldClose();
	void swapBuffers();
	GLFWwindow* getWindow();
};