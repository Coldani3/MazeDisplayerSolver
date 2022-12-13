#include "MazeMain.h"


int MazeMain::main() {
    loadMaze();
    setupGLFW();
    setupMonitor();

    window = std::make_shared<Window>(windowWidth, windowHeight, "Maze Displayer and Solver");

    setupRenderers();

    mazePathManager = std::make_shared<MazePathManager>();
    aiManager = std::make_shared<AIManager>(window, mazePathManager);//std::make_unique<AIManager>(maze, running);

    if (!checkWindowInitialised(*window) || !initialiseGLAD()) {
        std::cerr << "Failed GLAD and/or GLFW initialisation!" << '\n';
        
        glfwTerminate();

        return -1;
    }

    setupViewport();
    setupFramebufferCallback();

    std::thread aiThread([this]() {
        this->aiManager->run(this->maze, this->running);
    });

    aiThread.join();
    return 0;
}

#pragma region Setup
void MazeMain::loadMaze() {
    //Mazes can be several kilos of memory on their own - if not stored as pointers they may end up being inadvertently 
    //copied and eating memory
    maze = loadMazeFromFile("maze.cd3mazs");

    std::cout << "Maze loaded" << std::endl;
    std::cout << "Maze entrance coords: " << maze->mazeEntrance[0] << ", " << maze->mazeEntrance[1] << ", " << maze->mazeEntrance[2] << ", " << maze->mazeEntrance[3] << std::endl;
    std::cout << "Maze exit coords: " << maze->mazeExit[0] << ", " << maze->mazeExit[1] << ", " << maze->mazeExit[2] << ", " << maze->mazeExit[3] << std::endl;
}

std::shared_ptr<Maze> MazeMain::loadMazeFromFile(std::string path) {
    std::shared_ptr<Maze> out = std::make_shared<Maze>();
    out->loadFromFile(path);

    return maze;
}

void MazeMain::setupGLFW() noexcept {
    std::cout << "Initialising GLFW..." << std::endl;
    glfwInit();
}

void MazeMain::setupMonitor() noexcept {
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

void MazeMain::setupRenderers() noexcept {
    std::shared_ptr<MazeRenderInfo> mazeRenderInfo = std::make_shared<MazeRenderInfo>(0);

    std::cout << "Initialising renderers..." << std::endl;

    threeDRenderer = std::make_shared<MainRenderManager>(window, maze);
    threeDRenderer->mazeRenderInfo = mazeRenderInfo;

    guiRenderer = std::make_shared<GUIRenderManager>(maze, windowWidth, windowHeight);
    guiRenderer->mazeRenderInfo = mazeRenderInfo;

    std::cout << "Done." << std::endl;
}

bool MazeMain::checkWindowInitialised(const Window& window) {
    return !(window.getWindow() == NULL);
}

bool MazeMain::initialiseGLAD() {
    return !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

void MazeMain::setupViewport() {
    glViewport(0, 0, threeDRenderer->getWidth(), threeDRenderer->getHeight());
}

void MazeMain::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    threeDRenderer->framebufferSizeCallback(window, width, height);
    guiRenderer->framebufferSizeCallback(window, width, height);
}

void MazeMain::setupFramebufferCallback() {
    glfwSetFramebufferSizeCallback(threeDRenderer->window->getWindow(), [=](GLFWwindow* window, int width, int height) {
        framebufferSizeCallback(window, width, height); 
    });
}

#pragma endregion

#pragma region Main Exectuion
#pragma endregion