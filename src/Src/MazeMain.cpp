#include "MazeMain.h"
#include "AI/AIManager.h"

int MazeMain::main() {
    loadMaze();
    setupGLFW();
    setupMonitor();

    window = std::make_shared<Window>(windowWidth, windowHeight, "Maze Displayer and Solver");

    setupRenderers();

    aiManager = std::make_unique<AIManager>(maze, running);
    return 0;
}

#pragma region Setup
void MazeMain::loadMaze() {
    //Mazes can get pretty big in memory so pointers are the call here.
    maze = std::make_shared<Maze>();
    maze->loadFromFile("maze.cd3mazs");

    std::cout << "Maze loaded" << std::endl;
    std::cout << "Maze entrance coords: " << maze->mazeEntrance[0] << ", " << maze->mazeEntrance[1] << ", " << maze->mazeEntrance[2] << ", " << maze->mazeEntrance[3] << std::endl;
    std::cout << "Maze exit coords: " << maze->mazeExit[0] << ", " << maze->mazeExit[1] << ", " << maze->mazeExit[2] << ", " << maze->mazeExit[3] << std::endl;
}

void MazeMain::setupGLFW() {
    //initialise it here as renderer needs to be not null
    std::cout << "Initialising GLFW..." << std::endl;
    //initialise glfw
    glfwInit();
}

void MazeMain::setupMonitor() {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    //tell glfw we're using opengl 3.3 with the core profile, instead of the old method
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    glfwGetMonitorWorkarea(monitor, &windowXPos, &windowYPos, &windowWidth, &windowHeight);
}

void MazeMain::setupRenderers() {
    std::shared_ptr<MazeRenderInfo> mazeRenderInfo = std::make_shared<MazeRenderInfo>(0);

    std::cout << "Initialising renderers..." << std::endl;

    threeDRenderer = std::make_shared<MainRenderManager>(window, maze);
    threeDRenderer->mazeRenderInfo = mazeRenderInfo;

    guiRenderer = std::make_shared<GUIRenderManager>(maze, windowWidth, windowHeight);
    guiRenderer->mazeRenderInfo = mazeRenderInfo;

    std::cout << "Done." << std::endl;
}

#pragma endregion
