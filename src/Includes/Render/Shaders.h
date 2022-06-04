#ifndef MAZE_SHADERS
#define MAZE_SHADERS

#pragma region Maze
#pragma region Vertex Shaders
//TODO: implement shrinking based on transit between two 4d slices
//https://stackoverflow.com/a/14197892
const char* const mazeCellPathVertexShader = R"glsl(
#version 330 core

layout (location = 0) in vec3 cubeCoord;
layout (location = 1) in vec3 vertexNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalTransform;

out vec3 vecNormal;
out vec3 modelVertexPassed;

void main() {
	vec4 cubeVec4 = vec4(cubeCoord, 1.0);
	vecNormal = normalize(normalTransform * vertexNormal);
	modelVertexPassed = vec3(model * cubeVec4);
	gl_Position = projection * view * model * cubeVec4;
})glsl";

//stride = sizeof(float) * 3
// size: 3
//offset: 0
const char* const mazeCellCenterCubeVertexShader = R"glsl(
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
const char* const mazeCellCenterCubeFragmentShader = R"glsl(
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

//pretty much a copy but I'll probably do something with it eventually
const char* const mazeCellPathFragmentShader = R"glsl(
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

#pragma endregion

#pragma region UI Shaders
#pragma region Vertex

const char* const fourDIndicatorVertexShader = R"glsl(
#version 330 core

layout (location = 0) in vec3 squareVector;

uniform mat4 model;
uniform mat4 projection;

void main() {
	gl_Position = projection * model * vec4(squareVector, 1.0);
}
)glsl";

#pragma endregion

#pragma region Fragment

const char* const fourDIndicatorFragmentShader = R"glsl(
#version 330 core

uniform vec4 squareColour;

out vec4 FragColor;

void main() {
	FragColor = squareColour;
}
)glsl";

#pragma endregion


#pragma endregion
#pragma endregion

#endif