#pragma once

#include <Maze/Maze.h>
#include <Maze/MazePath.h>
#include <Render/PerspectiveCamera.h>
#include <Render/Renderer.h>

#include <vector>
#include <array>
#include "../Maze/MazePathManager.h"
#include "../Maze/MazePathRenderProgress.h"
#include "../Maths/Coordinate.h"
#include "Buffers/VBO.h"
#include "Buffers/VAO.h"
#include "Buffers/VBOConfiguration.h"
#include "ShaderProgram.h"
//#include <glm/gtx/transform.hpp>

class MazeRenderer : public Renderer {
public:
    /*
     * The path that has been selected to be gradually displayed (ie. NOT the one that is displayed) 
     */
    std::shared_ptr<MazePathManager> pathManager;

    /*
     * The path that describes what is being shown to the user and is highlighted gradually. 
     */
    std::shared_ptr<MazePathRenderProgress> renderedPathProgress = nullptr;
    //bool hasActiveRenderedPath = false;
    double lastPathAddTime = 0.0;
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
    const std::vector<float> mazePathVerticesNormals = {
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
    const std::vector<float> cubeVerticesNormals = {
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
    const glm::vec3 defaultLightColour = glm::vec3(1.0f, 1.0f, 1.0f);

	MazeRenderer(std::shared_ptr<PerspectiveCamera> camera, std::shared_ptr<Maze> maze, std::shared_ptr<MazePathManager> pathManager, int centerX, int centerY, int centerZ);
	~MazeRenderer();

	void render(std::shared_ptr<MazeRenderInfo> mazeRenderInfo);
    void progressPath(double now, MazeRenderInfo& mazeRenderInfo);
	void setup();
    void precomputeCellPathTransformations();
	void cleanup();
	void setShowPath(bool showPath);
    void changeShownPathTo(const MazePath& newPath);
    virtual void getRenderPollInput(GLFWwindow* window, double delta, const InputManager& inputManager) override;
    virtual std::shared_ptr<Camera> getCamera() const override;

	glm::vec3 getCellColour(const Coordinate<int>& coords) const;
	glm::mat4 mazeCellPathTransform(const glm::vec3& initialCoords, const glm::mat4& transformation) const;

	void drawMazeCellCenter(const Coordinate<int>& coords);
	void drawMazeCellPaths(unsigned char mazeCellData, const Coordinate<int>& coords, int lastW, float transitionScale);
    bool drawMazeCellPath(unsigned char mazeCellData, unsigned char prevWData, unsigned int cellPath, const glm::mat4& initialTranslate, const glm::vec3& modelCoords, const Coordinate<int>& mazeCoords, float transitionScale);

    void updateTransition(const unsigned char data, float& scale, double now, MazeRenderInfo& mazeRenderInfo, const Coordinate<int>& coords);
    constexpr float calculateScale(float endTransitionTime, float now, float mazeTransitionAnimationSpeed) const;

private:
    std::shared_ptr<Maze> maze = nullptr;
    std::shared_ptr<PerspectiveCamera> camera = nullptr;
    bool show4DIndicators = true;
    double lastPathShowChange = 0;
    double lastIndicatorToggle = 0;

    const std::vector<Coordinate<int>> touchingSides = {
        Coordinate<int>({1, 0, 0, 0}),
        Coordinate<int>({-1, 0, 0, 0}),
        Coordinate<int>({0, 1, 0, 0}),
        Coordinate<int>({0, -1, 0, 0}),
        Coordinate<int>({0, 0, 1, 0}),
        Coordinate<int>({0, 0, -1, 0}),
        Coordinate<int>({0, 0, 0, 1}),
        Coordinate<int>({0, 0, 0, -1})
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
    ShaderProgram cellCenterProgram = ShaderProgram("Maze Cell Center");
    ShaderProgram mazePathProgram = ShaderProgram("Maze path");
	//int cellCenterProgram;
	//int mazePathProgram;

    VBOf mazeCenterVBO;
    VAO<float> mazeCenterVAO;

    VBOf mazePathVBO;
    VAO<float> mazePathVAO;

	/*unsigned int mazeCenterVBO;
	unsigned int mazePathVBO;

	unsigned int mazeCenterVAO;
	unsigned int mazePathVAO;*/
#pragma endregion

    glm::vec3 coordsFromMazeCenter(int mazeX, int mazeY, int mazeZ) const;
    void prepMazeCenterDraw(const glm::mat4& model, const glm::mat4& view, const glm::vec3& cellColour);
    void useMazePathProgram(const glm::vec3& lightPos, const glm::vec3& lightColour);
    glm::mat3 calculateNormalTransform(const glm::mat4& model) const;
    void prepMazeDrawUniforms(const glm::vec3& cellColour, const glm::mat4& model, const glm::mat3& normalTransform);
    inline float calculateAdjustedScale(unsigned char prevWData, unsigned char mazeCellData, unsigned char bitChecking, int i, float transitionScale) const;
    inline bool hasCellPathBit(unsigned char mazeCellData, unsigned char bitChecking) const;

    void setupShaders();
    void createBuffers();
    void createCubeBuffers();
    void createPathBuffers();
};

