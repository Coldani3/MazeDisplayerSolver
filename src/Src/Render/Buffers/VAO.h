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
	VAO() {
		glGenVertexArrays(1, &vao);
	}

	void bind() const {
		glBindVertexArray(vao);
	}

	constexpr unsigned int getVAO() const noexcept {
		return vao;
	}

	bool operator==(const VAO& vao) {
		return vao.vao == this->vao;
	}

	static void unbindVAOs() {
		glBindVertexArray(0);
	}
};

#endif