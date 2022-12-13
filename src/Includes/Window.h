#ifndef __gl_h
#include <glad/glad.h>
#endif

#include <glfw3/glfw3.h>
#include <string>

#pragma once
//Wrapper around the GLFWwindow* object and various GLFW window functions.
class Window {
	GLFWwindow* window;
	int width, height, defaultWidth, defaultHeight;
public:
	Window(int width, int height, std::string title);

	void setWindowTitle(std::string title);
	bool shouldClose() const;
	void setShouldClose();
	void swapBuffers();
	GLFWwindow* getWindow() const;
};