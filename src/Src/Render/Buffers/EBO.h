//#ifndef CD3_RENDER_EBO
//#define CD3_RENDER_EBO

#include <glad/glad.h>
#include <glfw3/glfw3.h>
#include <vector>

#pragma once
class EBO
{
	unsigned int ebo;

public:
	EBO() {
		glGenBuffers(1, &ebo);
	}

	EBO(EBO&& moving) noexcept {
		ebo = std::exchange(moving.ebo, 0);
	}

	//do not copy. we do not want the destructor to delete the
	//ebo by accident
	EBO(const EBO& copying) = delete;

	constexpr unsigned int getEBO() const noexcept {
		return ebo;
	}

	void bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	}

	void loadData(std::vector<int> data, GLenum usage) const {
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(int), data.data(), usage);
	}

	bool operator==(const EBO& ebo) const {
		return ebo.ebo == this->ebo;
	}
};

//#endif