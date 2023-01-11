#ifndef CD3_RENDER_VAO
#define CD3_RENDER_VAO

#include <glad/glad.h>
#include <glfw3/glfw3.h>
#include <vector>

#pragma once
template <typename T>
class VAO {
	unsigned int vao;
public:
	VAO();
	void bind() const;
	constexpr unsigned int getVAO() const noexcept;

	static void unbindVAOs() {
		glBindVertexArray(0);
	}
};

#endif