#pragma once

#include <Maze/Maze.h>
#include <Maze/MazePath.h>
#include <Render/Camera.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

class MazeRenderer
{
public:
	MazePath selectedPath;
	bool showPath;
	float mazeCenterX = 500.0f;
	float mazeCenterY = 500.0f;
	float mazeCenterZ = 500.0f;

#pragma region GL_Consts
    /* Kept for reference

    unsigned int cuboidIndices[] = {
        0, 1, 3, //right face upper triangle
        0, 2, 3, //right face lower triangle
        0, 4, 6, //front face upper triangle
        0, 2, 6, //front face lower triangle
        0, 1, 5, //top face furthest triangle
        0, 4, 5, //top face closest triangle
        2, 6, 7, //bottom face closest triangle
        2, 3, 7, //bottom face furthest tirangle
        3, 7, 5, //back face bottom triangle
        3, 1, 5, //back face top triangle
        6, 7, 5, //left face bottom face
        6, 4, 5  //left face top face
    };*/

    const float cubeVerticesNormals[216] = {
        //right face
        0.0625f, 0.0625f, 0.0625f, 1.0f, 0.0f, 0.0f,
        0.0625f, 0.0625f, -0.0625f, 1.0f, 0.0f, 0.0f,
        0.0625f, -0.0625f, -0.0625f, 1.0f, 0.0f, 0.0f,
        0.0625f, 0.0625f, 0.0625f, 1.0f, 0.0f, 0.0f,
        0.0625f, -0.0625f, 0.0625f, 1.0f, 0.0f, 0.0f,
        0.0625f, -0.0625f, -0.0625f, 1.0f, 0.0f, 0.0f,

        //front face [back face?]
        0.0625f, 0.0625f, 0.0625f, 0.0f, 0.0f, 1.0f,
        -0.0625f, 0.0625f, 0.0625f, 0.0f, 0.0f, 1.0f,
        -0.0625f, -0.0625f, 0.0625f, 0.0f, 0.0f, 1.0f,
        0.0625f, 0.0625f, 0.0625f, 0.0f, 0.0f, 1.0f,
        -0.0625f, -0.0625f, 0.0625f, 0.0f, 0.0f, 1.0f,
        0.0625f, -0.0625f, 0.0625f, 0.0f, 0.0f, 1.0f,

        //top face
        0.0625f, 0.0625f, 0.0625f, 0.0f, 1.0f, 0.0f,
        0.0625f, 0.0625f, -0.0625f, 0.0f, 1.0f, 0.0f,
        -0.0625f, 0.0625f, -0.0625f, 0.0f, 1.0f, 0.0f,
        0.0625f, 0.0625f, 0.0625f, 0.0f, 1.0f, 0.0f,
        -0.0625f, 0.0625f, 0.0625f, 0.0f, 1.0f, 0.0f,
        -0.0625f, 0.0625f, -0.0625f, 0.0f, 1.0f, 0.0f,

        //bottom face
        0.0625f, -0.0625f, 0.0625f, 0.0f, -1.0f, 0.0f,
        -0.0625f, -0.0625f, 0.0625f, 0.0f, -1.0f, 0.0f,
        -0.0625f, -0.0625f, -0.0625f, 0.0f, -1.0f, 0.0f,
        0.0625f, -0.0625f, 0.0625f, 0.0f, -1.0f, 0.0f,
        -0.0625f, -0.0625f, -0.0625f, 0.0f, -1.0f, 0.0f,
        0.0625f, -0.0625f, -0.0625f, 0.0f, -1.0f, 0.0f,

        //back face [front face?]
        0.0625f, -0.0625f, -0.0625f, 0.0f, 0.0f, -1.0f,
        -0.0625f, -0.0625f, -0.0625f, 0.0f, 0.0f, -1.0f,
        -0.0625f, 0.0625f, -0.0625f, 0.0f, 0.0f, -1.0f,
        0.0625f, -0.0625f, -0.0625f, 0.0f, 0.0f, -1.0f,
        0.0625f, 0.0625f, -0.0625f, 0.0f, 0.0f, -1.0f,
        -0.0625f, 0.0625f, -0.0625f, 0.0f, 0.0f, -1.0f,

        //left face
        -0.0625f, -0.0625f, 0.0625f, -1.0f, 0.0f, 0.0f,
        -0.0625f, -0.0625f, -0.0625f, -1.0f, 0.0f, 0.0f,
        -0.0625f, 0.0625f, -0.0625f, -1.0f, 0.0f, 0.0f,
        -0.0625f, -0.0625f, 0.0625f, -1.0f, 0.0f, 0.0f,
        -0.0625f, 0.0625f, -0.0625f, -1.0f, 0.0f, 0.0f,
        -0.0625f, 0.0625f, 0.0625f, -1.0f, 0.0f, 0.0f
    };

    //float mazePathVertices[] = {
    //    0.0625, 0.0625, 0.1625, //0
    //    0.0625, 0.0625, -0.1625, //1
    //    0.0625, -0.0625, 0.1625, //2
    //    0.0625, -0.0625, -0.1625, //3
    //    -0.0625, 0.0625, 0.1625, //4
    //    -0.0625, 0.0625, -0.1625, //5
    //    -0.0625, -0.0625, 0.1625, //6
    //    -0.0625, -0.0625, -0.1625 //7
    //};

    //0.0625 = half of cube
    //0.0625 * 2 for top and bottom of division = 0.125
    //1 - 0.125 = 0.875 = length of path in total
    //0.875 / 2 = 0.4375 = length of each individual path piece
    //0.4375 / 2 = 0.21875
    const float mazePathVerticesNormals[216] = {
        //right
        0.0625f, 0.0625f, 0.21875f, 1.0f, 0.0f, 0.0f,
        0.0625f, 0.0625f, -0.21875f, 1.0f, 0.0f, 0.0f,
        0.0625f, -0.0625f, -0.21875f, 1.0f, 0.0f, 0.0f,
        0.0625f, 0.0625f, 0.21875f, 1.0f, 0.0f, 0.0f,
        0.0625f, -0.0625f, -0.21875f, 1.0f, 0.0f, 0.0f,
        0.0625f, -0.0625f, 0.21875f, 1.0f, 0.0f, 0.0f,
        //front
        0.0625f, 0.0625f, 0.21875f, 0.0f, 0.0f, 1.0f,
        -0.0625f, 0.0625f, 0.21875f, 0.0f, 0.0f, 1.0f,
        -0.0625f, -0.0625f, 0.21875f, 0.0f, 0.0f, 1.0f,
        0.0625f, 0.0625f, 0.21875f, 0.0f, 0.0f, 1.0f,
        -0.0625f, -0.0625f, 0.21875f, 0.0f, 0.0f, 1.0f,
        0.0625f, -0.0625f, 0.21875f, 0.0f, 0.0f, 1.0f,
        //top
        0.0625f, 0.0625f, 0.21875f, 0.0f, 1.0f, 0.0f,
        -0.0625f, 0.0625f, -0.21875f, 0.0f, 1.0f, 0.0f,
        0.0625f, 0.0625f, -0.21875f, 0.0f, 1.0f, 0.0f,
        0.0625f, 0.0625f, 0.21875f, 0.0f, 1.0f, 0.0f,
        -0.0625f, 0.0625f, 0.21875f, 0.0f, 1.0f, 0.0f,
        -0.0625f, 0.0625f, -0.21875f, 0.0f, 1.0f, 0.0f,
        //bottom
        0.0625f, -0.0625f, 0.21875f, 0.0f, -1.0f, 0.0f,
        -0.0625f, -0.0625f, 0.21875f, 0.0f, -1.0f, 0.0f,
        -0.0625f, -0.0625f, -0.21875f, 0.0f, -1.0f, 0.0f,
        0.0625f, -0.0625f, 0.21875f, 0.0f, -1.0f, 0.0f,
        -0.0625f, -0.0625f, -0.21875f, 0.0f, -1.0f, 0.0f,
        0.0625f, -0.0625f, -0.21875f, 0.0f, -1.0f, 0.0f,
        //back
        0.0625f, -0.0625f, -0.21875f, 0.0f, 0.0f, -1.0f,
        -0.0625f, 0.0625f, -0.21875f, 0.0f, 0.0f, -1.0f,
        -0.0625f, -0.0625f, -0.21875f, 0.0f, 0.0f, -1.0f,
        0.0625f, -0.0625f, -0.21875f, 0.0f, 0.0f, -1.0f,
        0.0625f, 0.0625f, -0.21875f, 0.0f, 0.0f, -1.0f,
        -0.0625f, 0.0625f, -0.21875f, 0.0f, 0.0f, -1.0f,
        //left
        -0.0625f, -0.0625f, 0.21875f, -1.0f, 0.0f, 0.0f,
        -0.0625f, -0.0625f, -0.21875f, -1.0f, 0.0f, 0.0f,
        -0.0625f, 0.0625f, -0.21875f, -1.0f, 0.0f, 0.0f,
        -0.0625f, -0.0625f, 0.21875f, -1.0f, 0.0f, 0.0f,
        -0.0625f, 0.0625f, -0.21875f, -1.0f, 0.0f, 0.0f,
        -0.0625f, 0.0625f, 0.21875f, -1.0f, 0.0f, 0.0f
    };
#pragma endregion

    //xRot, yRot, xTrans, yTrans, zTrans - front = 0 deg
//0.0625 + 0.21875 = 0.28125
    const std::vector<std::vector<float>> cellPathTransformations = {
        {0.0f, 90.0f, 0.0f, 0.28125f, 0.0f},  //up 
        {0.0f, -90.0f, 0.0f, -0.28125f, 0.0f}, //down  
        {270.0f, 0.0f, -0.28125f, 0.0f, 0.0f}, //left
        {90.0f, 0.0f, 0.28125f, 0.0f, 0.0f}, //right
        {180.0f, 0.0f, 0.0f, 0.0f, 0.28125f},//back
        {0.0f, 0.0f, 0.0f, 0.0f, -0.28125f}, //front
        //to corner of cube = 0.0625 in all directions
        //0.8 * 0.21875 = 0.175
        //
        {45.0f, 35.0f, 0.0625, 0.0625f, -0.0625f}, //ana
        {-135.0f, -35.0f, -0.0625, -0.0625f, 0.0625f} //kata

    };

    const glm::vec3 defaultCellColour = glm::vec3(0.54f, 0.54f, 0.54f);
    //177, 3, 252 - purpley
    const glm::vec3 visitedCellColour = glm::vec3(0.694f, 0.0117f, 0.988f);
    const glm::vec3 mazeEntranceColour = glm::vec3(0.0f, 1.0f, 0.0f);
    const glm::vec3 mazeExitColour = glm::vec3(1.0f, 0.0f, 0.0f);
    //229, 203, 85 - yellowy
    const glm::vec3 anaColour = glm::vec3(0.901f, 0.796f, 0.333f);
    //101, 106, 201 - bluey
    const glm::vec3 kataColour = glm::vec3(0.396f, 0.415f, 0.788f);

	MazeRenderer(Maze maze, int centerX, int centerY, int centerZ);
	~MazeRenderer();

	void render();
	void setup();
	void cleanup();
	void setCamera(std::shared_ptr<Camera> camera);
	void setMazeCenterProgram(int program);
	void setMazePathProgram(int program);
	void setShowPath(bool showPath);
	void setWViewing(int w);
	int getWViewing();

	glm::vec3 getCellColour(std::vector<int> coords);
	glm::mat4 mazeCellPathTransform(glm::vec3 initialCoords, float rotateAngleX, float rotateAngleY, float translateX, float translateY, float translateZ);

	void drawMazeCellCenter(int mazeX, int mazeY, int mazeZ, int mazeW);
	void drawMazeCellPaths(unsigned char mazeCellData, int mazeX, int mazeY, int mazeZ, int mazeW);

private:
	Maze maze;
	int currentW = 0;
#pragma region GL_Vars
	int cellCenterProgram;
	int mazePathProgram;
	unsigned int mazeCenterVBO;
	unsigned int mazePathVBO;

	unsigned int mazeCenterVAO;
	unsigned int mazePathVAO;
#pragma endregion

#pragma region Maze_Logic_Vars
	std::shared_ptr<Camera> camera;
#pragma endregion
};

