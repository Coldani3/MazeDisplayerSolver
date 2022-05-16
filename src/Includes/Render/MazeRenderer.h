#pragma once

#include <Maze/Maze.h>
#include <Maze/MazePath.h>
//#include <Render/RenderManager.h>
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

	MazeRenderer(Maze maze, int centerX, int centerY, int centerZ);
	~MazeRenderer();

	void render();
	void setup();
	void cleanup();
	void setCamera(std::shared_ptr<Camera> camera);
	void setMazeCenterProgram(int program);
	void setMazePathProgram(int program);
	void setShowPath(bool showPath);

	glm::vec3 getCellColour(std::vector<int> coords);
	glm::mat4 getViewMatrixFromCamera();
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

