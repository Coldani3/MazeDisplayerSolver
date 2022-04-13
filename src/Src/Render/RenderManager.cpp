#include <glad/glad.h>
#include <Render/RenderManager.h>
#include <Render/Shaders.h>
#include <iostream>
#include <memory>
#include <vector>

#include <glm/glm.hpp>
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
    0.175, 0.175, 0.175, 1.0, 0.0, 0.0,
    0.175, 0.175, -0.175, 1.0, 0.0, 0.0,
    0.175, -0.175, -0.175, 1.0, 0.0, 0.0,
    0.175, 0.175, 0.175, 1.0, 0.0, 0.0,
    0.175, -0.175, 0.175, 1.0, 0.0, 0.0,
    0.175, -0.175, -0.175, 1.0, 0.0, 0.0,

    //front face [back face?]
    0.175, 0.175, 0.175, 0.0, 0.0, 1.0,
    -0.175, 0.175, 0.175, 0.0, 0.0, 1.0,
    -0.175, -0.175, 0.175, 0.0, 0.0, 1.0,
    0.175, 0.175, 0.175, 0.0, 0.0, 1.0,
    -0.175, -0.175, 0.175, 0.0, 0.0, 1.0,
    0.175, -0.175, 0.175, 0.0, 0.0, 1.0,

    //top face
    0.175, 0.175, 0.175, 0.0, 1.0, 0.0,
    0.175, 0.175, -0.175, 0.0, 1.0, 0.0,
    -0.175, 0.175, -0.175, 0.0, 1.0, 0.0,
    0.175, 0.175, 0.175, 0.0, 1.0, 0.0,
    -0.175, 0.175, 0.175, 0.0, 1.0, 0.0,
    -0.175, 0.175, -0.175, 0.0, 1.0, 0.0,

    //bottom face
    0.175, -0.175, 0.175, 0.0, -1.0, 0.0,
    -0.175, -0.175, 0.175, 0.0, -1.0, 0.0,
    -0.175, -0.175, -0.175, 0.0, -1.0, 0.0,
    0.175, -0.175, 0.175, 0.0, -1.0, 0.0,
    -0.175, -0.175, -0.175, 0.0, -1.0, 0.0,
    0.175, -0.175, -0.175, 0.0, -1.0, 0.0,

    //back face [front face?]
    0.175, -0.175, -0.175, 0.0, 0.0, -1.0,
    -0.175, -0.175, -0.175, 0.0, 0.0, -1.0,
    -0.175, 0.175, -0.175, 0.0, 0.0, -1.0,
    0.175, -0.175, -0.175, 0.0, 0.0, -1.0,
    0.175, 0.175, -0.175, 0.0, 0.0, -1.0,
    -0.175, 0.175, -0.175, 0.0, 0.0, -1.0,

    //left face
    -0.175, -0.175, 0.175, -1.0, 0.0, 0.0,
    -0.175, -0.175, -0.175, -1.0, 0.0, 0.0,
    -0.175, 0.175, -0.175, -1.0, 0.0, 0.0,
    -0.175, -0.175, 0.175, -1.0, 0.0, 0.0,
    -0.175, 0.175, -0.175, -1.0, 0.0, 0.0,
    -0.175, 0.175, 0.175, -1.0, 0.0, 0.0
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

//0.175 = half of cube
//0.175 * 2 for top and bottom of division = 0.35
//1 - 0.35 = 0.65 = length of path in total
//0.65 / 2 = 0.325 = length of each individual path piece
//0.325 / 2 = 0.1625
float mazePathVerticesNormals[] = {
    //right
    0.0625, 0.0625, 0.1625, 1.0, 0.0, 0.0,
    0.0625, 0.0625, -0.1625, 1.0, 0.0, 0.0,
    0.0625, -0.0625, -0.1625, 1.0, 0.0, 0.0,
    0.0625, 0.0625, 0.1625, 1.0, 0.0, 0.0,
    0.0625, -0.0625, -0.1625, 1.0, 0.0, 0.0,
    0.0625, -0.0625, 0.1625, 1.0, 0.0, 0.0,
    //front
    0.0625, 0.0625, 0.1625, 0.0, 0.0, 1.0,
    -0.0625, 0.0625, 0.1625, 0.0, 0.0, 1.0,
    -0.0625, -0.0625, 0.1625, 0.0, 0.0, 1.0,
    0.0625, 0.0625, 0.1625, 0.0, 0.0, 1.0,
    -0.0625, -0.0625, 0.1625, 0.0, 0.0, 1.0,
    0.0625, -0.0625, 0.1625, 0.0, 0.0, 1.0,
    //top
    0.0625, 0.0625, 0.1625, 0.0, 1.0, 0.0,
    -0.0625, 0.0625, -0.1625, 0.0, 1.0, 0.0,
    0.0625, 0.0625, -0.1625, 0.0, 1.0, 0.0,
    0.0625, 0.0625, 0.1625, 0.0, 1.0, 0.0,
    -0.0625, 0.0625, 0.1625, 0.0, 1.0, 0.0,
    -0.0625, 0.0625, -0.1625, 0.0, 1.0, 0.0,
    //bottom
    0.0625, -0.0625, 0.1625, 0.0, -1.0, 0.0,
    -0.0625, -0.0625, 0.1625, 0.0, -1.0, 0.0,
    -0.0625, -0.0625, -0.1625, 0.0, -1.0, 0.0,
    0.0625, -0.0625, 0.1625, 0.0, -1.0, 0.0,
    -0.0625, -0.0625, -0.1625, 0.0, -1.0, 0.0,
    0.0625, -0.0625, -0.1625, 0.0, -1.0, 0.0,
    //back
    0.0625, -0.0625, -0.1625, 0.0, 0.0, -1.0,
    -0.0625, 0.0625, -0.1625, 0.0, 0.0, -1.0,
    -0.0625, -0.0625, -0.1625, 0.0, 0.0, -1.0,
    0.0625, -0.0625, -0.1625, 0.0, 0.0, -1.0,
    0.0625, 0.0625, -0.1625, 0.0, 0.0, -1.0,
    -0.0625, 0.0625, -0.1625, 0.0, 0.0, -1.0,
    //left
    -0.0625, -0.0625, 0.1625, -1.0, 0.0, 0.0,
    -0.0625, -0.0625, -0.1625, -1.0, 0.0, 0.0,
    -0.0625, 0.0625, -0.1625, -1.0, 0.0, 0.0,
    -0.0625, -0.0625, 0.1625, -1.0, 0.0, 0.0,
    -0.0625, 0.0625, -0.1625, -1.0, 0.0, 0.0,
    -0.0625, 0.0625, 0.1625, -1.0, 0.0, 0.0
};

//xRot, yRot, xTrans, yTrans, zTrans - front = 0 deg
//0.175 + 0.1625 = 0.3375
std::vector<std::vector<float>> cellPathTransformations = {
    {0.0f, 90.0f, 0.0f, 0.3375f, 0.0f}, //up
    {0.0f, -90.0f, 0.0f, -0.3375f, 0.0f}, //down
    {90.0f, 0.0f, 0.3375f, 0.0f, 0.0f}, //right
    {270.0f, 0.0f, -0.3375f, 0.0f, 0.0f}, //left
    {0.0f, 0.0f, 0.0f, 0.0f, -0.3375f}, //front
    {180.0f, 0.0f, 0.0f, 0.0f, 0.3375f}  //back
};

float centerX = 500;
float centerY = 500;
float centerZ = 500;

#pragma endregion

#pragma region Non-Class Methods
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

#pragma endregion

#pragma region Class Methods
RenderManager::RenderManager(int width, int height) {
    //create the window
    window = glfwCreateWindow(width, height, "Maze Displayer and Solver", NULL, NULL);

    glfwMakeContextCurrent(window);

    defaultWidth = width;
    defaultHeight = height;
    this->width = width;
    this->height = height;

    projection = glm::perspective(glm::radians(45.0f), (float) this->width / (float) this->height, 0.1f, 100.0f);
    camera = std::make_unique<Camera>(Camera(centerX, centerY, centerZ + -15.0f, centerX, centerY, centerZ));
}

RenderManager::~RenderManager() {
    std::cout << "Beginning Renderer Cleanup..." << std::endl;
    std::cout << "Clearing GL Programs..." << std::endl;
    //I think openGL deletes these automatically but it's better to be safe than sorry
    glDeleteProgram(genericCubeProgram);
    glDeleteProgram(cellCenterProgram);

    std::cout << "Renderer Cleanup done." << std::endl;
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
    currentW = w;
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
        glm::vec3 coords = glm::vec3(mazeX + centerX, mazeY + centerY, mazeZ + centerZ);
        glm::mat4 model = translateModel(coords);
        glm::mat4 view = /*glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -6.0f));*/ getViewMatrixFromCamera();

        glm::vec3 cellColour = glm::vec3(0.54f, 0.54f, 0.54f);
        glm::vec3 lightPos = glm::vec3(camera->getXPos(), camera->getYPos(), camera->getZPos());
        glm::vec3 lightColour = glm::vec3(1.0f, 1.0f, 1.0f);

        glUniformMatrix4fv(glGetUniformLocation(cellCenterProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(cellCenterProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(cellCenterProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glUniform3fv(glGetUniformLocation(cellCenterProgram, "cellColour"), 1, glm::value_ptr(cellColour));
        glUniform3fv(glGetUniformLocation(cellCenterProgram, "lightPos"), 1, glm::value_ptr(lightPos));
        glUniform3fv(glGetUniformLocation(cellCenterProgram, "lightColour"), 1, glm::value_ptr(lightColour));

        glUseProgram(cellCenterProgram);
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
        glm::vec3 coords = glm::vec3(mazeX + centerX, mazeY + centerY, mazeZ + centerZ);
        //translation to get it to the same coords as the center piece, from which we then translate it again into the proper position
        glm::mat4 initialTranslate = translateModel(coords);
        glm::mat4 view = getViewMatrixFromCamera();
        float wLerp = 1.0f;

        glm::vec3 cellColour = glm::vec3(0.54f, 0.54f, 0.54f);
        glm::vec3 lightPos = glm::vec3(camera->getXPos(), camera->getYPos(), camera->getZPos());
        glm::vec3 lightColour = glm::vec3(1.0f, 1.0f, 1.0f);

        glUniform3fv(glGetUniformLocation(genericCubeProgram, "cellColour"), 1, glm::value_ptr(cellColour));
        glUniform3fv(glGetUniformLocation(genericCubeProgram, "lightPos"), 1, glm::value_ptr(lightPos));
        glUniform3fv(glGetUniformLocation(genericCubeProgram, "lightColour"), 1, glm::value_ptr(lightColour));

        glUniformMatrix4fv(glGetUniformLocation(genericCubeProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(genericCubeProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glUseProgram(genericCubeProgram);
        glBindVertexArray(rectangleCubeVAO);

        for (unsigned int i = 0; i < 8; i++) {
            unsigned int bitChecking = 1 << i;

            if (i < 6 && (mazeCellData & bitChecking) > 0) {
                std::vector<float> transformation = cellPathTransformations[i];
                glm::mat4 model = mazeCellPathTransform(coords, transformation[0], transformation[1], transformation[2], transformation[3], transformation[4]) * initialTranslate;
                
                //TODO: move all matrix multiplications into the shaders?
                glUniformMatrix4fv(glGetUniformLocation(genericCubeProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }

        //TODO: to drawarray
        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

void RenderManager::setup() {
    std::cout << "Setting up OpenGL..." << std::endl;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);

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
    
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &rectangleCubeVBO);
    glGenVertexArrays(1, &cubeVAO);

    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerticesNormals), cubeVerticesNormals, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

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

void RenderManager::draw() {
    glClearColor(0.8470f, 0.8823f, 0.9098f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (int x = 0; x < 10; x++) {
        for (int y = 0; y < 10; y++) {
            for (int z = 0; z < 10; z++) {
                drawMazeCellCenter(x, y, z, 0);
                //drawMazeCellPaths(63, x, y, z, 0);
            }
        }
    }
    drawMazeCellPaths(63, 0, 0, 0, 0);
    drawMazeCellPaths(63, 1, 0, 0, 0);
    drawMazeCellPaths(63, 0, 0, 1, 0);
    //drawMazeCellCenter(1, 1, 1, 0);
}
#pragma endregion