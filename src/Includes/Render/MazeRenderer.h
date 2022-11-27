#pragma once

#include <Maze/Maze.h>
#include <Maze/MazePath.h>
#include <Render/PerspectiveCamera.h>
#include <Render/Renderer.h>

#include <vector>
#include <array>
//#include <glm/gtx/transform.hpp>

class MazeRenderer : public Renderer {
public:
    /*
     * The path that has been selected to be gradually displayed (ie. NOT the one that is displayed) 
     * TODO: these really should be smart pointers
     */
	MazePath selectedPath;

    /*
     * The path that describes what is being shown to the user and is highlighted gradually. 
     */
    MazePath renderedPath;
    double lastPathAddTime = 0;
    //Index of selected path to be rendered.
    int selectedPathIndex = 0;
    /* Describes how many times the path 'updates' (ie. is copied from the result to the output gradually)
     * per second with formula (1 / p).
     * e.g 0.1f = 10 updates per second
     */
    const float pathUpdateSpeed = 0.1f;
	bool showPath;
    //set true to track to see if last frame the transition animation was happening.
    bool wasTransitioning = false;
	float mazeCenterX = 500.0f;
	float mazeCenterY = 500.0f;
	float mazeCenterZ = 500.0f;

#pragma region GL_Consts
    const float cubeVerticesNormals[216] = {
        //back face
        -0.0625f, -0.0625f, -0.0625f, 0.0f, 0.0f, -1.0f,
        0.0625f, 0.0625f, -0.0625f, 0.0f, 0.0f, -1.0f,
        0.0625f, -0.0625f, -0.0625f, 0.0f, 0.0f, -1.0f,
        0.0625f, 0.0625f, -0.0625f, 0.0f, 0.0f, -1.0f,
        -0.0625f, -0.0625f, -0.0625f, 0.0f, 0.0f, -1.0f,
        -0.0625f, 0.0625f, -0.0625f, 0.0f, 0.0f, -1.0f,

        //front face
        -0.0625f, -0.0625f, 0.0625f, 0.0f, 0.0f, 1.0f,
        0.0625f, -0.0625f, 0.0625f, 0.0f, 0.0f, 1.0f,
        0.0625f, 0.0625f, 0.0625f, 0.0f, 0.0f, 1.0f,
        -0.0625f, 0.0625f, 0.0625f, 0.0f, 0.0f, 1.0f,
        -0.0625f, -0.0625f, 0.0625f, 0.0f, 0.0f, 1.0f,
        0.0625f, 0.0625f, 0.0625f, 0.0f, 0.0f, 1.0f,

        //left face
        -0.0625f, 0.0625f, -0.0625f, -1.0f, 0.0f, 0.0f,
        -0.0625f, -0.0625f, -0.0625f, -1.0f, 0.0f, 0.0f,
        -0.0625f, -0.0625f, 0.0625f, -1.0f, 0.0f, 0.0f,
        -0.0625f, 0.0625f, 0.0625f, -1.0f, 0.0f, 0.0f,
        -0.0625f, 0.0625f, -0.0625f, -1.0f, 0.0f, 0.0f,
        -0.0625f, -0.0625f, 0.0625f, -1.0f, 0.0f, 0.0f,

        //right face
        0.0625f, -0.0625f, -0.0625f, 1.0f, 0.0f, 0.0f,
        0.0625f, 0.0625f, -0.0625f, 1.0f, 0.0f, 0.0f,
        0.0625f, 0.0625f, 0.0625f, 1.0f, 0.0f, 0.0f,
        0.0625f, 0.0625f, 0.0625f, 1.0f, 0.0f, 0.0f,
        0.0625f, -0.0625f, 0.0625f, 1.0f, 0.0f, 0.0f,
        0.0625f, -0.0625f, -0.0625f, 1.0f, 0.0f, 0.0f,

        //bottom face
        0.0625f, -0.0625f, 0.0625f, 0.0f, -1.0f, 0.0f,
        -0.0625f, -0.0625f, 0.0625f, 0.0f, -1.0f, 0.0f,
        -0.0625f, -0.0625f, -0.0625f, 0.0f, -1.0f, 0.0f,
        0.0625f, -0.0625f, 0.0625f, 0.0f, -1.0f, 0.0f,
        -0.0625f, -0.0625f, -0.0625f, 0.0f, -1.0f, 0.0f,
        0.0625f, -0.0625f, -0.0625f, 0.0f, -1.0f, 0.0f,
        
        //top face
        0.0625f, 0.0625f, 0.0625f, 0.0f, 1.0f, 0.0f,
        0.0625f, 0.0625f, -0.0625f, 0.0f, 1.0f, 0.0f,
        -0.0625f, 0.0625f, -0.0625f, 0.0f, 1.0f, 0.0f,
        -0.0625f, 0.0625f, -0.0625f, 0.0f, 1.0f, 0.0f,
        -0.0625f, 0.0625f, 0.0625f, 0.0f, 1.0f, 0.0f,
        0.0625f, 0.0625f, 0.0625f, 0.0f, 1.0f, 0.0f
    };

    /*TODO: frontand back are never displayed anywhere other than on the 4D paths.get rid of them
      with these and then make a new set of buffers for the 4D paths.*/

    /*0.0625 = half of cube
      0.0625 * 2 for top and bottom of division = 0.125
      1 - 0.125 = 0.875 = length of path in total
      0.875 / 2 = 0.4375 = length of each individual path piece
      0.4375 / 2 = 0.21875*/
    const float mazePathVerticesNormals[216] = {
        //right
        0.0625f, -0.0625f, -0.21875f, 1.0f, 0.0f, 0.0f,
        0.0625f, 0.0625f, -0.21875f, 1.0f, 0.0f, 0.0f,
        0.0625f, 0.0625f, 0.21875f, 1.0f, 0.0f, 0.0f,
        0.0625f, -0.0625f, 0.21875f, 1.0f, 0.0f, 0.0f,
        0.0625f, -0.0625f, -0.21875f, 1.0f, 0.0f, 0.0f,
        0.0625f, 0.0625f, 0.21875f, 1.0f, 0.0f, 0.0f,
        //front
        0.0625f, 0.0625f, 0.21875f, 0.0f, 0.0f, 1.0f,
        -0.0625f, 0.0625f, 0.21875f, 0.0f, 0.0f, 1.0f,
        -0.0625f, -0.0625f, 0.21875f, 0.0f, 0.0f, 1.0f,
        0.0625f, 0.0625f, 0.21875f, 0.0f, 0.0f, 1.0f,
        -0.0625f, -0.0625f, 0.21875f, 0.0f, 0.0f, 1.0f,
        0.0625f, -0.0625f, 0.21875f, 0.0f, 0.0f, 1.0f,
        //top
        0.0625f, 0.0625f, -0.21875f, 0.0f, 1.0f, 0.0f,
        -0.0625f, 0.0625f, -0.21875f, 0.0f, 1.0f, 0.0f,
        0.0625f, 0.0625f, 0.21875f, 0.0f, 1.0f, 0.0f,
        -0.0625f, 0.0625f, -0.21875f, 0.0f, 1.0f, 0.0f,
        -0.0625f, 0.0625f, 0.21875f, 0.0f, 1.0f, 0.0f,
        0.0625f, 0.0625f, 0.21875f, 0.0f, 1.0f, 0.0f,
        //bottom
        0.0625f, -0.0625f, 0.21875f, 0.0f, -1.0f, 0.0f,
        -0.0625f, -0.0625f, 0.21875f, 0.0f, -1.0f, 0.0f,
        -0.0625f, -0.0625f, -0.21875f, 0.0f, -1.0f, 0.0f,
        0.0625f, -0.0625f, 0.21875f, 0.0f, -1.0f, 0.0f,
        -0.0625f, -0.0625f, -0.21875f, 0.0f, -1.0f, 0.0f,
        0.0625f, -0.0625f, -0.21875f, 0.0f, -1.0f, 0.0f,
        //back
        -0.0625f, -0.0625f, -0.21875f, 0.0f, 0.0f, -1.0f,
        -0.0625f, 0.0625f, -0.21875f, 0.0f, 0.0f, -1.0f,
        0.0625f, -0.0625f, -0.21875f, 0.0f, 0.0f, -1.0f,
        -0.0625f, 0.0625f, -0.21875f, 0.0f, 0.0f, -1.0f,
        0.0625f, 0.0625f, -0.21875f, 0.0f, 0.0f, -1.0f,
        0.0625f, -0.0625f, -0.21875f, 0.0f, 0.0f, -1.0f,
        //left
        -0.0625f, 0.0625f, -0.21875f, -1.0f, 0.0f, 0.0f,
        -0.0625f, -0.0625f, -0.21875f, -1.0f, 0.0f, 0.0f,
        -0.0625f, -0.0625f, 0.21875f, -1.0f, 0.0f, 0.0f,
        -0.0625f, 0.0625f, 0.21875f, -1.0f, 0.0f, 0.0f,
        -0.0625f, 0.0625f, -0.21875f, -1.0f, 0.0f, 0.0f,
        -0.0625f, -0.0625f, 0.21875f, -1.0f, 0.0f, 0.0f
    };
#pragma endregion

    //xRot, yRot, xTrans, yTrans, zTrans - front = 0 deg
//0.0625 + 0.21875 = 0.28125
    const std::vector<std::array<float, 5>> cellPathTransformationsValues = {
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

    std::array<glm::mat4, 8> cellPathTransformations;

    const glm::vec3 defaultCellColour = glm::vec3(0.54f, 0.54f, 0.54f);
    //177, 3, 252 - purpley
    const glm::vec3 visitedCellColour = glm::vec3(0.694f, 0.0117f, 0.988f);
    const glm::vec3 mazeEntranceColour = glm::vec3(0.0f, 1.0f, 0.0f);
    const glm::vec3 mazeExitColour = glm::vec3(1.0f, 0.0f, 0.0f);
    //229, 203, 85 - yellowy
    const glm::vec3 anaColour = glm::vec3(0.901f, 0.796f, 0.333f);
    //101, 106, 201 - bluey
    const glm::vec3 kataColour = glm::vec3(0.396f, 0.415f, 0.788f);

	MazeRenderer(std::shared_ptr<PerspectiveCamera> camera, std::shared_ptr<Maze> maze, int centerX, int centerY, int centerZ);
	~MazeRenderer();

	void render(std::shared_ptr<MazeRenderInfo> mazeRenderInfo);
	void setup();
	void cleanup();
	void setMazeCenterProgram(int program);
	void setMazePathProgram(int program);
	void setShowPath(bool showPath);
    virtual void getRenderPollInput(GLFWwindow* window, double delta) override;
    virtual std::shared_ptr<Camera> getCamera() override;

	glm::vec3 getCellColour(std::vector<int> coords);
	glm::mat4 mazeCellPathTransform(glm::vec3 initialCoords, glm::mat4 transformation);

	void drawMazeCellCenter(int mazeX, int mazeY, int mazeZ, int mazeW);
	void drawMazeCellPaths(unsigned char mazeCellData, int mazeX, int mazeY, int mazeZ, int mazeW, int lastW, float transitionScale);

private:
    std::shared_ptr<Maze> maze = nullptr;
    std::shared_ptr<PerspectiveCamera> camera = nullptr;
    bool show4DIndicators = true;
    double lastPathShowChange = 0;
    double lastIndicatorToggle = 0;

    const std::vector<std::vector<int>> touchingSides = {
        {1, 0, 0, 0},
        {-1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, -1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, -1, 0},
        {0, 0, 0, 1},
        {0, 0, 0, -1}
    };

    const std::vector<std::vector<int>> touchingSidesOpposite = {
        {-1, 0, 0, 0},
        {1, 0, 0, 0},
        {0, -1, 0, 0},
        {0, 1, 0, 0},
        {0, 0, -1, 0},
        {0, 0, 1, 0},
        {0, 0, 0, -1},
        {0, 0, 0, 1}
    };

#pragma region GL_Vars
	int cellCenterProgram;
	int mazePathProgram;

	unsigned int mazeCenterVBO;
	unsigned int mazePathVBO;

	unsigned int mazeCenterVAO;
	unsigned int mazePathVAO;
#pragma endregion
};

