#ifndef __gl_h
#include <glad/glad.h>
#endif

#include <glfw3/glfw3.h>
#include <memory>
#include "Window.h"

#pragma once
class InputManager
{
	std::shared_ptr<Window> window;

public:
	InputManager(std::shared_ptr<Window> window);
	int getKey(int key) noexcept;
	bool getKeyPressed(int key) noexcept;
};

