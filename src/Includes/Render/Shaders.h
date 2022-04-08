#pragma once

#pragma region Vertex Shaders
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
layout (location = 1) in vec3 vertexNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vecNormal;
out vec3 modelVertexPassed;

void main() {
	vec4 cubeVec4 = vec4(cubeCoord, 1.0);
	vecNormal = vertexNormal;
	modelVertexPassed = vec3(model * cubeVec4);
	gl_Position = projection * view * model * cubeVec4;
})glsl";

#pragma endregion

#pragma region Geometry Shaders

//normalization code from: https://learnopengl.com/Advanced-OpenGL/Geometry-Shader (accessed 08/04/2022)
/*const char* normalsGeometryShader = R"glsl(
#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
	vec3 modelVertex;
} gs_in[];

out vec3 vecNormal;
out vec3 modelVertexPassed;

vec3 getNormal() {
	//get parallel lines to triangle surface with vector subtraction
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);

	//use cross product to get normal and normalize it into a unit vector
	return normalize(cross(a, b));
}

void main() {
	vecNormal = getNormal();

	//pass through, we only want to calculate the normal
	gl_Position = gl_in[0].gl_Position;
	modelVertexPassed = gs_in[0].modelVertex;
	EmitVertex();
	gl_Position = gl_in[1].gl_Position;
	modelVertexPassed = gs_in[1].modelVertex;
	EmitVertex();
	gl_Position = gl_in[2].gl_Position;
	modelVertexPassed = gs_in[2].modelVertex;
	EmitVertex();
	EndPrimitive();
})glsl";*/

#pragma endregion

#pragma region Fragment Shaders
const char *mazeCellCenterCubeFragmentShader = R"glsl(
#version 330 core

out vec4 FragColor;
in vec3 modelVertexPassed;
in vec3 vecNormal;

uniform vec3 cellColour;
uniform vec3 lightPos;
uniform vec3 lightColour;

void main() {
	//just in case
	vec3 normal = normalize(vecNormal);
	vec3 lightDirection = normalize(lightPos - modelVertexPassed);

	vec3 ambient = 0.1 * lightColour;
	vec3 diffuse = max(dot(normal, lightDirection), 0.0) * lightColour;
	FragColor = vec4((diffuse + ambient) * cellColour, 1.0);
})glsl";

const char *mazeCubeFragmentShader = R"glsl(
#version 330 core

out vec4 FragColor;

void main() {
	FragColor = vec4(0.54f, 0.54f, 0.54f, 1.0f);
})glsl";

#pragma endregion