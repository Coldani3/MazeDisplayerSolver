#include <Render/Buffers/VAO.h>

template <typename T>
VAO<T>::VAO() {
	glGenVertexArrays(1, &vao);
}

template <typename T>
void VAO<T>::bind() const {
	glBindVertexArray(vao);
}

template <typename T>
constexpr unsigned int VAO<T>::getVAO() const noexcept {
	return vao;
}

template<typename T>
bool VAO<T>::operator==(const VAO& vao) {
	return vao.vao == this->vao;
}
