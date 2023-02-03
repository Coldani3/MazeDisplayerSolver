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
	VBO();
	~VBO();
	VBO<T, DataType>& addConfiguration(const VBOConfiguration& config);
	VBO<T, DataType>& loadData(const VAO<T>& vao, const std::vector<T>& data, GLenum usage);
	VBO<T, DataType>& loadEBOData(const EBO& ebo, const std::vector<int>& data, GLenum usage);
	VBO<T, DataType>& initialise();
	void bind();

	bool operator==(const VBO<T, DataType>& vbo);

	constexpr unsigned int getVbo() const noexcept;
};

using VBOf = VBO<float, GL_FLOAT>;
using VBOi = VBO<int, GL_INT>;

//#endif