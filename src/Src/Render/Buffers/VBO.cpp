#include "VBO.h"

template<typename T, GLenum DataType>
VBO<T, DataType>::VBO() {
	glGenBuffers(1, &vbo);
}

template<typename T, GLenum DataType>
VBO<T, DataType>::~VBO() {
	glDeleteBuffers(1, &vbo);
}

template<typename T, GLenum DataType>
VBO<T, DataType>& VBO<T, DataType>::loadData(const VAO<T>& vao, const std::vector<T>& data, GLenum usage) {
	vao.bind();
	bind();
	glBufferData(GL_ARRAY_BUFFER, data.size() * typeSize, data.data(), usage);
	return *this;
}

template<typename T, GLenum DataType>
VBO<T, DataType>& VBO<T, DataType>::loadEBOData(const EBO& ebo, const std::vector<int>& data, GLenum usage) {
	ebo.bind();
	ebo.loadData(data, usage);
	return *this;
}

template<typename T, GLenum DataType>
VBO<T, DataType>& VBO<T, DataType>::addConfiguration(const VBOConfiguration& config) {
	this->config.push_back(config);
	return *this;
}

template<typename T, GLenum DataType>
VBO<T, DataType>& VBO<T, DataType>::initialise() {
	int dataQuantity = 0;

	for (const VBOConfiguration& vboConfig : config) {
		dataQuantity += vboConfig.size;
	}

	size_t stride = typeSize * dataQuantity;
	int dataSoFar = 0;

	for (int i = 0; i < config.size(); ++i) {
		VBOConfiguration& vboConfig = config[i];
		glVertexAttribPointer(i, vboConfig.size, DataType, vboConfig.normalize, stride, reinterpret_cast<void*>(typeSize * dataSoFar));
		glEnableVertexAttribArray(i);
		dataSoFar += vboConfig.size;
	}

	return *this;
}

template<typename T, GLenum DataType>
void VBO<T, DataType>::bind() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

template<typename T, GLenum DataType>
bool VBO<T, DataType>::operator==(const VBO<T, DataType>& vbo) {
	return vbo.vbo == this->vbo && 
		std::equal(vbo.config.begin(), vbo.config.end(), this->config.begin(), this->config.end());
}

template<typename T, GLenum DataType>
constexpr unsigned int VBO<T, DataType>::getVbo() const noexcept {
	return vbo;
}