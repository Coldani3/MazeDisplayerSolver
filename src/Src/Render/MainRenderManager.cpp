#include "MainRenderManager.h"
#include "MazeRenderer.h"

#pragma region Class Methods
MainRenderManager::MainRenderManager(std::shared_ptr<Window> window, std::shared_ptr<Maze> maze, std::shared_ptr<MazePathManager> pathManager) {
    //Moved to Window
    this->window = window;
    
    this->maze = maze;

    std::vector<float> mazeCenter = { 
        centerX + (static_cast<float>(maze->width) / 2.0f), 
        centerY + (static_cast<float>(maze->height) / 2),
        centerZ + (static_cast<float>(maze->depth) / 2)
    };

    //this looks wacky :/ maybe go back to making camera a pointer
    camera = std::make_shared<PerspectiveCamera>(
        mazeCenter[0], 
        mazeCenter[1], 
        mazeCenter[2] + -15.0f, 
        defaultWidth, 
        defaultHeight, 
        mazeCenter[0], 
        mazeCenter[1], 
        mazeCenter[2]
    );

    //std::shared_ptr<PerspectiveCamera> perspCam = std::make_shared<PerspectiveCamera>(mazeCenter[0], mazeCenter[1], mazeCenter[2] + -15.0f, defaultWidth, defaultHeight, mazeCenter[0], mazeCenter[1], mazeCenter[2]);
    camera->lookAt(mazeCenter[0], mazeCenter[1], mazeCenter[2]);

    mazeRenderer = std::make_unique<MazeRenderer>(camera, maze, pathManager, centerX, centerY, centerZ);
    //std::cout << "t" << std::endl;

    projection = glm::perspective(
        glm::radians(45.0f), 
        static_cast<float>(this->width) / static_cast<float>(this->height), 
        0.1f, 
        100.0f
    );
}

void MainRenderManager::setMazeUsing(std::shared_ptr<Maze> maze) {
    this->maze = maze;
}

std::shared_ptr<Camera> MainRenderManager::getCamera() const {
    return camera;
}

void MainRenderManager::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    this->width = width;
    this->height = height;
    camera->setScreenSize(width, height);
}

int MainRenderManager::getWidth() const {
    return width;
}

int MainRenderManager::getHeight() const {
    return height;
}

void MainRenderManager::setup() {
    std::cout << "Setting up OpenGL..." << std::endl;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);

    mazeRenderer->setup();

    std::cout << "Finished setting up OpenGL." << std::endl;
}

void MainRenderManager::render() {
    glClearColor(0.8470f, 0.8823f, 0.9098f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mazeRenderer->render(mazeRenderInfo);
}
#pragma endregion