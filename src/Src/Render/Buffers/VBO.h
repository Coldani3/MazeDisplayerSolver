//#ifndef CD3_RENDER_VBO
//#define CD3_RENDER_VBO

#include <glad/glad.h>
#include <glfw3/glfw3.h>
#include <vector>
#include "VBOConfiguration.h"
#include "VAO.h"
#include "EBO.h"

#pragma once
//Example: VBO<float, GL_FLOAT>
template <typename T, GLenum DataType>
class VBO
{
	unsigned int vbo;
	std::vector<VBOConfiguration> config;
	const size_t typeSize = sizeof(T);

public:
	VBO() {
		glGenBuffers(1, &vbo);
	}

	//do not copy. might accidentally delete the VBO when a
	//copy deconstructs
	VBO(const VBO& copy) = delete;
	VBO(VBO&& moving) {
		vbo = std::exchange(moving.vbo, 0);
		config = std::move(moving.config);
	}

	~VBO() {
		glDeleteBuffers(1, &vbo);
	}

	VBO<T, DataType>& addConfiguration(const VBOConfiguration& config) {
		this->config.push_back(config);
		return *this;
	}

	VBO<T, DataType>& loadData(const VAO<T>& vao, const std::vector<T>& data, GLenum usage) {
		vao.bind();
		bind();
		glBufferData(GL_ARRAY_BUFFER, data.size() * typeSize, data.data(), usage);
		return *this;
	}

	VBO<T, DataType>& loadEBOData(const EBO& ebo, const std::vector<int>& data, GLenum usage) {
		ebo.bind();
		ebo.loadData(data, usage);
		return *this;
	}

	VBO<T, DataType>& initialise() {
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

	void bind() {
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
	}

	bool operator==(const VBO<T, DataType>& vbo) {
		return vbo.vbo == this->vbo &&
			std::equal(vbo.config.begin(), vbo.config.end(), this->config.begin(), this->config.end());
	}

	constexpr unsigned int getVbo() const noexcept {
		return vbo;
	}
};

using VBOf = VBO<float, GL_FLOAT>;
using VBOi = VBO<int, GL_INT>;

//#endif