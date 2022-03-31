#pragma once

const char *genericCubeVertexShader = R"glsl(
#version 330 core

layout (location = 0) in vec3 cubeVec;

void main() {
	gl_Position = vec4(cubeVec, 1.0);
})glsl";

//stride = sizeof(float) * 3
// size: 3
//offset: 0
const char *mazeCellCenterCubeVertexShader = R"glsl(
#version 330 core

layout (location = 0) in vec3 cubeCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * model * vec4(cubeCoord, 1.0);
})glsl";

const char *mazeCellCenterCubeFragmentShader = R"glsl(
#version 330 core

out vec4 FragColor;

uniform vec3 cellColour;

void main() {
	FragColor = vec4(cellColour, 1.0);
})glsl";

const char *mazeCubeFragmentShader = R"glsl(
#version 330 core

out vec4 FragColor;

void main() {
	FragColor = vec4(0.54f, 0.54f, 0.54f, 1.0f);
})glsl";

