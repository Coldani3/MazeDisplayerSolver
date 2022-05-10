#include <glfw3/glfw3.h>

#include <glm/glm.hpp>

#include <memory>
#include <Render/Camera.h>
#include <Maze/Maze.h>

#pragma once
class RenderManager
{
public:
	RenderManager(int width, int height, Maze initialMaze);
	~RenderManager();
	void setup();
	void draw();
	void framebufferSizeCallback(GLFWwindow* window, int width, int height);
	GLFWwindow* getWindow();
	int getWidth();
	int getHeight();
	void drawMazeCellCenter(int mazeX, int mazeY, int mazeZ = 0, int mazeW = 0);
	void drawMazeCellPaths(unsigned char mazeCellData, int mazeX, int mazeY, int mazeZ = 0, int mazeW = 0);
	void setWViewing(int w);
	int getWViewing();
	void setMazeUsing(Maze maze);
	std::unique_ptr<Camera> getCamera();
	glm::mat4 mazeCellPathTransform(glm::vec3 initialCoords, float rotateAngleX, float rotateAngleY, float translateX, float translateY, float translateZ);
	glm::mat4 getViewMatrixFromCamera();
	glm::vec3 getCellColour(std::vector<int> coords);
	void markCellVisited(std::vector<int> coords);
	void clearVisitedCells();
	void setCellHeadOfSolver(std::vector<int> coords);
	bool visitedCell(std::vector<int> coords);

	glm::mat4 projection;
	std::unique_ptr<Camera> camera;
	std::vector<bool> visited;
	std::vector<int> head;
private:
	GLFWwindow* window;
	int width;
	int height;
	int defaultWidth;
	int defaultHeight;
	int currentW = 0;
	Maze maze;
};

