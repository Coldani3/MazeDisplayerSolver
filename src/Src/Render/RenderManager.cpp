#include <glad/glad.h>
#include <Render/RenderManager.h>
#include <Render/Shaders.h>
#include <iostream>

/*unsigned int genericCubeShaderVert;
unsigned int cellCenterShaderVert;
unsigned int genericCubeShaderFrag;
unsigned int cellCenterShaderFrag;*/

unsigned int genericCubeProgram;
unsigned int cellCenterProgram;

unsigned int cuboidEBO;


RenderManager::RenderManager(int width, int height) {
    //create the window
    window = glfwCreateWindow(width, height, "Maze Displayer and Solver", NULL, NULL);

    glfwMakeContextCurrent(window);

    defaultWidth = width;
    defaultHeight = height;
    this->width = width;
    this->height = height;
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

void RenderManager::setup() {
    //create shaders
    unsigned int genericCubeShaderVert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(genericCubeShaderVert, 1, &genericCubeVertexShader, NULL);
    glCompileShader(genericCubeShaderVert);
    checkShaderCompileSuccess(genericCubeShaderVert);

    unsigned int genericCubeShaderFrag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(genericCubeShaderFrag, 1, &mazeCubeFragmentShader, NULL);
    glCompileShader(genericCubeShaderFrag);
    checkShaderCompileSuccess(genericCubeShaderFrag);

    unsigned int cellCenterCubeShaderVert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(cellCenterCubeShaderVert, 1, &mazeCellCenterCubeVertexShader, NULL);
    glCompileShader(cellCenterCubeShaderVert);
    checkShaderCompileSuccess(cellCenterCubeShaderVert);

    unsigned int cellCenterCubeShaderFrag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(cellCenterCubeShaderFrag, 1, &mazeCellCenterCubeFragmentShader, NULL);
    glCompileShader(cellCenterCubeShaderFrag);
    checkShaderCompileSuccess(cellCenterCubeShaderFrag);

    //create programs
    genericCubeProgram = glCreateProgram();
    glAttachShader(genericCubeProgram, genericCubeShaderVert);
    glAttachShader(genericCubeProgram, genericCubeShaderFrag);
    glLinkProgram(genericCubeProgram);
    checkProgramCompileSuccess(genericCubeProgram);

    cellCenterProgram = glCreateProgram();
    glAttachShader(cellCenterProgram, cellCenterCubeShaderVert);
    glAttachShader(cellCenterProgram, cellCenterCubeShaderFrag);
    glLinkProgram(cellCenterProgram);
    checkProgramCompileSuccess(cellCenterProgram);

    //clean up unecessary shaders
    glDeleteShader(genericCubeShaderVert);
    glDeleteShader(genericCubeShaderFrag);
    glDeleteShader(cellCenterCubeShaderVert);
    glDeleteShader(cellCenterCubeShaderFrag);

    //setup buffers and shader programs
}

void RenderManager::draw() {
    glClearColor(0.8470f, 0.8823f, 0.9098f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
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

void RenderManager::drawMazeCellCenter(int mazeX, int mazeY, int mazeZ, int mazeW = 0) {
    float vertices[] = {
        0.5, 0.5, 0.5, //top right front - 0
        0.5, 0.5, -0.5, //top right back - 1
        0.5, -0.5, 0.5, //bottom right front - 2
        0.5, -0.5, -0.5, //bottom right back - 3
        -0.5, 0.5, 0.5, //top left front - 4
        -0.5, 0.5, -0.5, //top left back - 5
        -0.5, -0.5, 0.5, //bottom left front - 6
        -0.5, -0.5, -0.5 //bottom left back - 7
    };

    unsigned int indices[] = {
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
    };
    unsigned int cubeVBO;
    unsigned int cubeVAO;

    glGenBuffers(1, &cubeVBO);
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cuboidEBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cuboidEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //TODO: move above to setup and rename the buffers to more fitting names.

    glUseProgram(cellCenterProgram);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}

void RenderManager::drawMazeCellPaths(unsigned char mazeCellData, int mazeX, int mazeY, int mazeZ = 0, int mazeW = 0) {

}
