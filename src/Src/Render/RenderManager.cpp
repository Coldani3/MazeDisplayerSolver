#include <glad/glad.h>
#include <Render/RenderManager.h>
#include <Render/Shaders.h>
#include <iostream>
#include <memory>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#pragma region Variables
int currentW = 0;

unsigned int genericCubeProgram;
unsigned int cellCenterProgram;

unsigned int cubeVBO;
unsigned int rectangleCubeVBO;

unsigned int cubeVAO;
unsigned int rectangleCubeVAO;

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

float cubeVerticesNormals[] = {
    //right face
    0.0625, 0.0625, 0.0625, 1.0, 0.0, 0.0,
    0.0625, 0.0625, -0.0625, 1.0, 0.0, 0.0,
    0.0625, -0.0625, -0.0625, 1.0, 0.0, 0.0,
    0.0625, 0.0625, 0.0625, 1.0, 0.0, 0.0,
    0.0625, -0.0625, 0.0625, 1.0, 0.0, 0.0,
    0.0625, -0.0625, -0.0625, 1.0, 0.0, 0.0,

    //front face [back face?]
    0.0625, 0.0625, 0.0625, 0.0, 0.0, 1.0,
    -0.0625, 0.0625, 0.0625, 0.0, 0.0, 1.0,
    -0.0625, -0.0625, 0.0625, 0.0, 0.0, 1.0,
    0.0625, 0.0625, 0.0625, 0.0, 0.0, 1.0,
    -0.0625, -0.0625, 0.0625, 0.0, 0.0, 1.0,
    0.0625, -0.0625, 0.0625, 0.0, 0.0, 1.0,

    //top face
    0.0625, 0.0625, 0.0625, 0.0, 1.0, 0.0,
    0.0625, 0.0625, -0.0625, 0.0, 1.0, 0.0,
    -0.0625, 0.0625, -0.0625, 0.0, 1.0, 0.0,
    0.0625, 0.0625, 0.0625, 0.0, 1.0, 0.0,
    -0.0625, 0.0625, 0.0625, 0.0, 1.0, 0.0,
    -0.0625, 0.0625, -0.0625, 0.0, 1.0, 0.0,

    //bottom face
    0.0625, -0.0625, 0.0625, 0.0, -1.0, 0.0,
    -0.0625, -0.0625, 0.0625, 0.0, -1.0, 0.0,
    -0.0625, -0.0625, -0.0625, 0.0, -1.0, 0.0,
    0.0625, -0.0625, 0.0625, 0.0, -1.0, 0.0,
    -0.0625, -0.0625, -0.0625, 0.0, -1.0, 0.0,
    0.0625, -0.0625, -0.0625, 0.0, -1.0, 0.0,

    //back face [front face?]
    0.0625, -0.0625, -0.0625, 0.0, 0.0, -1.0,
    -0.0625, -0.0625, -0.0625, 0.0, 0.0, -1.0,
    -0.0625, 0.0625, -0.0625, 0.0, 0.0, -1.0,
    0.0625, -0.0625, -0.0625, 0.0, 0.0, -1.0,
    0.0625, 0.0625, -0.0625, 0.0, 0.0, -1.0,
    -0.0625, 0.0625, -0.0625, 0.0, 0.0, -1.0,

    //left face
    -0.0625, -0.0625, 0.0625, -1.0, 0.0, 0.0,
    -0.0625, -0.0625, -0.0625, -1.0, 0.0, 0.0,
    -0.0625, 0.0625, -0.0625, -1.0, 0.0, 0.0,
    -0.0625, -0.0625, 0.0625, -1.0, 0.0, 0.0,
    -0.0625, 0.0625, -0.0625, -1.0, 0.0, 0.0,
    -0.0625, 0.0625, 0.0625, -1.0, 0.0, 0.0
};

float mazePathVertices[] = {
    0.0625, 0.0625, 0.1625, //0
    0.0625, 0.0625, -0.1625, //1
    0.0625, -0.0625, 0.1625, //2
    0.0625, -0.0625, -0.1625, //3
    -0.0625, 0.0625, 0.1625, //4
    -0.0625, 0.0625, -0.1625, //5
    -0.0625, -0.0625, 0.1625, //6
    -0.0625, -0.0625, -0.1625 //7
};

//0.0625 = half of cube
//0.0625 * 2 for top and bottom of division = 0.125
//1 - 0.125 = 0.875 = length of path in total
//0.875 / 2 = 0.4375 = length of each individual path piece
//0.4375 / 2 = 0.21875
float mazePathVerticesNormals[] = {
    //right
    0.0625, 0.0625, 0.21875, 1.0, 0.0, 0.0,
    0.0625, 0.0625, -0.21875, 1.0, 0.0, 0.0,
    0.0625, -0.0625, -0.21875, 1.0, 0.0, 0.0,
    0.0625, 0.0625, 0.21875, 1.0, 0.0, 0.0,
    0.0625, -0.0625, -0.21875, 1.0, 0.0, 0.0,
    0.0625, -0.0625, 0.21875, 1.0, 0.0, 0.0,
    //front
    0.0625, 0.0625, 0.21875, 0.0, 0.0, 1.0,
    -0.0625, 0.0625, 0.21875, 0.0, 0.0, 1.0,
    -0.0625, -0.0625, 0.21875, 0.0, 0.0, 1.0,
    0.0625, 0.0625, 0.21875, 0.0, 0.0, 1.0,
    -0.0625, -0.0625, 0.21875, 0.0, 0.0, 1.0,
    0.0625, -0.0625, 0.21875, 0.0, 0.0, 1.0,
    //top
    0.0625, 0.0625, 0.21875, 0.0, 1.0, 0.0,
    -0.0625, 0.0625, -0.21875, 0.0, 1.0, 0.0,
    0.0625, 0.0625, -0.21875, 0.0, 1.0, 0.0,
    0.0625, 0.0625, 0.21875, 0.0, 1.0, 0.0,
    -0.0625, 0.0625, 0.21875, 0.0, 1.0, 0.0,
    -0.0625, 0.0625, -0.21875, 0.0, 1.0, 0.0,
    //bottom
    0.0625, -0.0625, 0.21875, 0.0, -1.0, 0.0,
    -0.0625, -0.0625, 0.21875, 0.0, -1.0, 0.0,
    -0.0625, -0.0625, -0.21875, 0.0, -1.0, 0.0,
    0.0625, -0.0625, 0.21875, 0.0, -1.0, 0.0,
    -0.0625, -0.0625, -0.21875, 0.0, -1.0, 0.0,
    0.0625, -0.0625, -0.21875, 0.0, -1.0, 0.0,
    //back
    0.0625, -0.0625, -0.21875, 0.0, 0.0, -1.0,
    -0.0625, 0.0625, -0.21875, 0.0, 0.0, -1.0,
    -0.0625, -0.0625, -0.21875, 0.0, 0.0, -1.0,
    0.0625, -0.0625, -0.21875, 0.0, 0.0, -1.0,
    0.0625, 0.0625, -0.21875, 0.0, 0.0, -1.0,
    -0.0625, 0.0625, -0.21875, 0.0, 0.0, -1.0,
    //left
    -0.0625, -0.0625, 0.21875, -1.0, 0.0, 0.0,
    -0.0625, -0.0625, -0.21875, -1.0, 0.0, 0.0,
    -0.0625, 0.0625, -0.21875, -1.0, 0.0, 0.0,
    -0.0625, -0.0625, 0.21875, -1.0, 0.0, 0.0,
    -0.0625, 0.0625, -0.21875, -1.0, 0.0, 0.0,
    -0.0625, 0.0625, 0.21875, -1.0, 0.0, 0.0
};

//xRot, yRot, xTrans, yTrans, zTrans - front = 0 deg
//0.0625 + 0.21875 = 0.28125

std::vector<std::vector<float>> cellPathTransformations = {
    {0.0f, 90.0f, 0.0f, 0.28125f, 0.0f}, //front
    {0.0f, -90.0f, 0.0f, -0.28125f, 0.0f},  //back
    {270.0f, 0.0f, -0.28125f, 0.0f, 0.0f}, //right
    {90.0f, 0.0f, 0.28125f, 0.0f, 0.0f},//left
    {180.0f, 0.0f, 0.0f, 0.0f, 0.28125f}, //up 
    {0.0f, 0.0f, 0.0f, 0.0f, -0.28125f}, //down 
    //to corner of cube = 0.0625 in all directions
    //0.8 * 0.21875 = 0.175
    //
    {45.0f, 35.0f, 0.0625, 0.0625f, -0.0625f}, //ana
    {-135.0f, -35.0f, -0.0625, -0.0625f, 0.0625f} //kata
    
};

float centerX = 500;
float centerY = 500;
float centerZ = 500;

glm::vec3 defaultCellColour = glm::vec3(0.54f, 0.54f, 0.54f);
glm::vec3 mazeEntranceColour = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 mazeExitColour = glm::vec3(1.0f, 0.0f, 0.0f);
//229, 203, 85 - yellowy
glm::vec3 anaColour = glm::vec3(0.901f, 0.796f, 0.333f);
//101, 106, 201 - bluey
glm::vec3 kataColour = glm::vec3(0.396f, 0.415f, 0.788f);

#pragma endregion

#pragma region Non_Class_Methods
void checkShaderCompileSuccess(unsigned int shader) {
    int success;
    char infoLog[512];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "SHADER ERROR: " << infoLog << std::endl;
    }
}

void checkProgramCompileSuccess(unsigned int program) {
    int success;
    char infoLog[512];

    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "PROGRAM ERROR: " << infoLog << std::endl;
    }
}

glm::mat4 translateModel(glm::vec3 translation) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, translation);
    return model;
}

glm::mat4 translateModel(int x, int y, int z) {
    return translateModel(glm::vec3(x, y, z));
}

bool coordsMatch(std::vector<int> coords1, std::vector<int> coords2) {
    if (coords1.size() != coords2.size()) {
        std::cerr << "Non matching coordinate sizes detected!" << std::endl;
        return false;
    }

    for (int i = 0; i < coords1.size(); i++) {
        if (coords1[i] != coords2[i]) {
            return false;
        }
    }

    return true;
}

#pragma endregion

#pragma region Class Methods
RenderManager::RenderManager(int width, int height, Maze maze) {
    //create the window
    window = glfwCreateWindow(width, height, "Maze Displayer and Solver", NULL, NULL);

    glfwMakeContextCurrent(window);

    defaultWidth = width;
    defaultHeight = height;
    this->width = width;
    this->height = height;
    this->maze = maze;

    projection = glm::perspective(glm::radians(45.0f), (float) this->width / (float) this->height, 0.1f, 100.0f);
    camera = std::make_unique<Camera>(Camera(centerX, centerY, centerZ + -15.0f, centerX, centerY, centerZ));
    camera->lookAt(centerX + (maze.width / 2), centerY + (maze.height / 2), centerZ + (maze.depth / 2));
}

RenderManager::~RenderManager() {
    std::cout << "Beginning Renderer Cleanup..." << std::endl;
    std::cout << "Clearing GL Programs..." << std::endl;
    //I think openGL deletes these automatically but it's better to be safe than sorry
    glDeleteProgram(genericCubeProgram);
    glDeleteProgram(cellCenterProgram);

    std::cout << "Renderer Cleanup done." << std::endl;
}

void RenderManager::setMazeUsing(Maze maze) {
    this->maze = maze;
}

void RenderManager::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    this->width = width;
    this->height = height;
}

GLFWwindow* RenderManager::getWindow() {
    return window;
}

int RenderManager::getWidth() {
    return width;
}

int RenderManager::getHeight() {
    return height;
}

void RenderManager::setWViewing(int w) {
    if (w >= 0 && w < maze.hyperDepth) {
        currentW = w;
    }
}

int RenderManager::getWViewing() {
    return currentW;
}

glm::vec3 RenderManager::getCellColour(std::vector<int> coords) {
    if (coordsMatch(coords, maze.mazeEntrance)) {
        return mazeEntranceColour;
    } else if (coordsMatch(coords, maze.mazeExit)) {
        return mazeExitColour;
    } else {
        return defaultCellColour;
    }
}

glm::mat4 RenderManager::getViewMatrixFromCamera() {
    glm::vec3 camPos = glm::vec3(camera->getXPos(), camera->getYPos(), camera->getZPos());
    glm::vec3 lookingAtPos = glm::vec3(camera->getXLookingAt(), camera->getYLookingAt(), camera->getZLookingAt());

    glm::mat4 lookAtMat = glm::lookAt(camPos, lookingAtPos, glm::vec3(0.0f, 1.0f, 0.0f));

    return lookAtMat;
}

//translate is always last so we do that after this
glm::mat4 RenderManager::mazeCellPathTransform(glm::vec3 initialCoords, float rotateAngleX, float rotateAngleY, float translateX, float translateY, float translateZ) {
    glm::mat4 identity = glm::mat4(1.0f);

    glm::mat4 toOrigin = glm::translate(identity, -initialCoords);
    glm::mat4 andBack = glm::translate(identity, initialCoords);

    glm::mat4 rotateY = glm::rotate(identity, glm::radians(rotateAngleY), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotateX = glm::rotate(identity, glm::radians(rotateAngleX), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 translate = glm::translate(identity, glm::vec3(translateX, translateY, translateZ));

    return andBack * translate * rotateX * rotateY * toOrigin;
}

void RenderManager::drawMazeCellCenter(int mazeX, int mazeY, int mazeZ, int mazeW) {
    
    if (mazeW == currentW) {
        //TODO: store these vecs in a lookup buffer to save performance and memory
        glm::vec3 coords = glm::vec3((maze.width - mazeX) + centerX, mazeY + centerY, mazeZ + centerZ);
        glm::mat4 model = translateModel(coords);
        glm::mat4 view = getViewMatrixFromCamera();

        glm::vec3 lightPos = glm::vec3(camera->getXPos(), camera->getYPos(), camera->getZPos());
        glm::vec3 lightColour = glm::vec3(1.0f, 1.0f, 1.0f);

        glUseProgram(cellCenterProgram);

        glUniformMatrix4fv(glGetUniformLocation(cellCenterProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(cellCenterProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(cellCenterProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glUniform3fv(glGetUniformLocation(cellCenterProgram, "cellColour"), 1, glm::value_ptr(getCellColour({mazeX, mazeY, mazeZ, mazeW})));
        glUniform3fv(glGetUniformLocation(cellCenterProgram, "lightPos"), 1, glm::value_ptr(lightPos));
        glUniform3fv(glGetUniformLocation(cellCenterProgram, "lightColour"), 1, glm::value_ptr(lightColour));

        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //unbind VA
        glBindVertexArray(0);
    } else {
        std::cout << "Not in this 4D slice" << std::endl;
    }
}

void RenderManager::drawMazeCellPaths(unsigned char mazeCellData, int mazeX, int mazeY, int mazeZ, int mazeW) {
    if (mazeW == currentW) {
        std::vector<int> mazeCoords = { mazeX, mazeY, mazeZ, mazeW };
        glm::vec3 modelCoords = glm::vec3((maze.width - mazeX) + centerX, mazeY + centerY, mazeZ + centerZ);
        //translation to get it to the same modelCoords as the center piece, from which we then translate it again into the proper position
        glm::mat4 initialTranslate = translateModel(modelCoords);
        glm::mat4 view = getViewMatrixFromCamera();
        float wLerp = 1.0f;

        glm::vec3 lightPos = glm::vec3(camera->getXPos(), camera->getYPos(), camera->getZPos());
        glm::vec3 lightColour = glm::vec3(1.0f, 1.0f, 1.0f);

        glUseProgram(genericCubeProgram);

        glUniform3fv(glGetUniformLocation(genericCubeProgram, "lightPos"), 1, glm::value_ptr(lightPos));
        glUniform3fv(glGetUniformLocation(genericCubeProgram, "lightColour"), 1, glm::value_ptr(lightColour));

        glUniformMatrix4fv(glGetUniformLocation(genericCubeProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(genericCubeProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(rectangleCubeVAO);

        for (unsigned int i = 0; i < 8; i++) {
            unsigned int bitChecking = 1 << i;

            if ((mazeCellData & bitChecking) > 0) {
                std::vector<float> transformation = cellPathTransformations[i];
                //TODO: move all matrix multiplications into the shaders?
                glm::mat4 model = mazeCellPathTransform(modelCoords, transformation[0], transformation[1], transformation[2], transformation[3], transformation[4]) * initialTranslate;
                glm::vec3 cellColour = getCellColour(mazeCoords);

                if (i >= 6) {
                    if (bitChecking == ANA) {
                        cellColour = anaColour;
                    } else {
                        cellColour = kataColour;
                    }
                }

                glUniform3fv(glGetUniformLocation(genericCubeProgram, "cellColour"), 1, glm::value_ptr(cellColour));

                glUniformMatrix4fv(glGetUniformLocation(genericCubeProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }

        glBindVertexArray(0);
    }
}

void RenderManager::setup() {
    std::cout << "Setting up OpenGL..." << std::endl;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    //glEnable(GL_CULL_FACE);

    //create shaders
    std::cout << "Initialising Shaders..." << std::endl;
    std::cout << "genericCubeShaderVert..." << std::endl;
    unsigned int genericCubeShaderVert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(genericCubeShaderVert, 1, &mazeCellPathVertexShader, NULL);
    glCompileShader(genericCubeShaderVert);
    checkShaderCompileSuccess(genericCubeShaderVert);

    std::cout << "genericCubeShaderFrag..." << std::endl;
    unsigned int genericCubeShaderFrag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(genericCubeShaderFrag, 1, &mazeCellPathFragmentShader, NULL);
    glCompileShader(genericCubeShaderFrag);
    checkShaderCompileSuccess(genericCubeShaderFrag);

    std::cout << "cellCenterCubeShaderVert..." << std::endl;
    unsigned int cellCenterCubeShaderVert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(cellCenterCubeShaderVert, 1, &mazeCellCenterCubeVertexShader, NULL);
    glCompileShader(cellCenterCubeShaderVert);
    checkShaderCompileSuccess(cellCenterCubeShaderVert);

    std::cout << "cellCenterCubeShaderFrag..." << std::endl;
    unsigned int cellCenterCubeShaderFrag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(cellCenterCubeShaderFrag, 1, &mazeCellCenterCubeFragmentShader, NULL);
    glCompileShader(cellCenterCubeShaderFrag);
    checkShaderCompileSuccess(cellCenterCubeShaderFrag);

    /*unsigned int genericNormalsGeometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(genericNormalsGeometryShader, 1, &normalsGeometryShader, NULL);
    glCompileShader(genericNormalsGeometryShader);
    checkShaderCompileSuccess(genericNormalsGeometryShader);*/
    std::cout << "Done." << std::endl;

    //create programs
    std::cout << "Creating OpenGL Programs..." << std::endl;
    genericCubeProgram = glCreateProgram();
    glAttachShader(genericCubeProgram, genericCubeShaderVert);
    //glAttachShader(genericCubeProgram, genericNormalsGeometryShader);
    glAttachShader(genericCubeProgram, genericCubeShaderFrag);
    glLinkProgram(genericCubeProgram);
    checkProgramCompileSuccess(genericCubeProgram);

    cellCenterProgram = glCreateProgram();
    glAttachShader(cellCenterProgram, cellCenterCubeShaderVert);
    //glAttachShader(cellCenterProgram, genericNormalsGeometryShader);
    glAttachShader(cellCenterProgram, cellCenterCubeShaderFrag);
    glLinkProgram(cellCenterProgram);
    checkProgramCompileSuccess(cellCenterProgram);
    std::cout << "Done." << std::endl;

    std::cout << "Cleaning up Shaders..." << std::endl;
    //clean up unecessary shaders
    glDeleteShader(genericCubeShaderVert);
    glDeleteShader(genericCubeShaderFrag);
    glDeleteShader(cellCenterCubeShaderVert);
    glDeleteShader(cellCenterCubeShaderFrag);
    //glDeleteShader(genericNormalsGeometryShader);
    std::cout << "Done." << std::endl;

    // we will transform the cubes into the appropriate positions in the shader
    std::cout << "Creating and populating buffers..." << std::endl;
    
    //cube buffers
    glGenBuffers(1, &cubeVBO);
    glGenVertexArrays(1, &cubeVAO);

    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerticesNormals), cubeVerticesNormals, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //path buffers
    glGenBuffers(1, &rectangleCubeVBO);
    glGenVertexArrays(1, &rectangleCubeVAO);

    glBindVertexArray(rectangleCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, rectangleCubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mazePathVerticesNormals), mazePathVerticesNormals, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    std::cout << "Done." << std::endl;

    std::cout << "Finished setting up OpenGL." << std::endl;
}

void RenderManager::markCellVisited(std::vector<int> coords) {
    if (!(std::find(visited.begin(), visited.end(), coords) != visited.end())) {
        visited.push_back(coords);
    }
}

void RenderManager::clearVisitedCells() {
    visited.clear();
}

void RenderManager::setCellHeadOfSolver(std::vector<int> coords) {
    head = coords;
}

void RenderManager::draw() {
    glClearColor(0.8470f, 0.8823f, 0.9098f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (int x = 0; x < maze.width; x++) {
        for (int y = 0; y < maze.height; y++) {
            for (int z = 0; z < maze.depth; z++) {
                std::vector<int> coords = { x, y, z, currentW };

                if (maze[coords] > 0) {
                    drawMazeCellPaths(maze[coords], x, y, z, currentW);
                    drawMazeCellCenter(x, y, z, currentW);
                }
            }
        }
    }

    /*int debugCellData = 1;

    for (int x = 0; x < 68; x++) {
        drawMazeCellPaths(debugCellData, x, 0, 0, 0);
        debugCellData = debugCellData << 1;
    }

    drawMazeCellPaths(maze[maze.mazeEntrance], maze.mazeEntrance[0], maze.mazeEntrance[1], maze.mazeEntrance[2], maze.mazeEntrance[3]);
    drawMazeCellPaths(maze[maze.mazeExit], maze.mazeExit[0], maze.mazeExit[1], maze.mazeExit[2], maze.mazeExit[3]);*/

    /*drawMazeCellPaths(63, 0, 0, 0, 0);
    drawMazeCellPaths(63, 1, 0, 0, 0);
    drawMazeCellPaths(63, 0, 0, 1, 0);*/
    //drawMazeCellCenter(1, 1, 1, 0);
}
#pragma endregion