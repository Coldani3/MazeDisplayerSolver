#include <Render/Buffers/EBO.h>

EBO::EBO() {
	glGenBuffers(1, &ebo);
}

constexpr unsigned int EBO::getEBO() const noexcept {
	return ebo;
}

void EBO::bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}

void EBO::loadData(std::vector<int> data, GLenum usage) {
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(int), data.data(), usage);
}

bool EBO::operator==(const EBO& ebo) {
	return ebo.ebo == this->ebo;
}
