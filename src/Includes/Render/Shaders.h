#pragma once

const char *genericCubeVertexShader = R"glsl(
#version 330 core

layout (location = 0) in vec3 cubeVec;

void main() {
	gl_Position = cubeVec;
}
)glsl";

//stride = sizeof(float) * 3
// size: 3
//offset: 0
const char *mazeCellCenterCubeVertexShader = R"glsl(
#version 330 core

layout (location = 0) in vec3 cubeCoord;

void main() {
	gl_Position = cubeCoord;
}

)glsl";

const char *mazeCellCenterCubeFragmentShader = R"glsl(
#version 330 core

out vec4 FragColor;

uniform vec3 cellColour;

void main() {
	FragColor = cellColour;
}

)glsl";

const char *mazeCubeFragmentShader = R"glsl(
#version 330 core

out vec4 FragColor;

void main() {
	FragColor = vec4(0.54f, 0.54f, 0.54f, 1.0f);
}

)glsl";

