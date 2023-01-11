#ifndef CD3_RENDER_EBO
#define CD3_RENDER_EBO

#include <glad/glad.h>
#include <glfw3/glfw3.h>
#include <vector>

#pragma once
class EBO
{
	unsigned int ebo;

public:
	EBO();
	constexpr unsigned int getEBO() const noexcept;
	void bind();
	void loadData(std::vector<int> data, GLenum usage);
};

#endif