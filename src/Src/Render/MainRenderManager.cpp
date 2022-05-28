#include <Render/MainRenderManager.h>
#include <Render/MazeRenderer.h>

#pragma region Class Methods
MainRenderManager::MainRenderManager(int width, int height, std::shared_ptr<Maze> maze) {
    //create the window
    window = glfwCreateWindow(width, height, "Maze Displayer and Solver", NULL, NULL);
    //set to top left corner

    int left, top, right, bottom;
    glfwGetWindowFrameSize(window, &left, &top, &right, &bottom);

    glfwSetWindowSize(window, width, height - top);
    glfwSetWindowPos(window, 0, top);

    glfwMakeContextCurrent(window);

    defaultWidth = width;
    defaultHeight = height - top;
    this->width = width;
    this->height = height - top;
    this->maze = maze;

    std::vector<float> mazeCenter = { centerX + ((float)maze->width / 2), centerY + ((float)maze->height / 2), centerZ + ((float)maze->depth / 2) };

    camera = PerspectiveCamera(mazeCenter[0], mazeCenter[1], mazeCenter[2] + -15.0f, defaultWidth, defaultHeight, mazeCenter[0], mazeCenter[1], mazeCenter[2]);

    //std::shared_ptr<PerspectiveCamera> perspCam = std::make_shared<PerspectiveCamera>(mazeCenter[0], mazeCenter[1], mazeCenter[2] + -15.0f, defaultWidth, defaultHeight, mazeCenter[0], mazeCenter[1], mazeCenter[2]);
    camera.lookAt(mazeCenter[0], mazeCenter[1], mazeCenter[2]);

    mazeRenderer = std::make_unique<MazeRenderer>(camera, maze, centerX, centerY, centerZ);
    //std::cout << "t" << std::endl;

    projection = glm::perspective(glm::radians(45.0f), (float) this->width / (float) this->height, 0.1f, 100.0f);
}

MainRenderManager::~MainRenderManager() {
    std::cout << "Beginning Renderer Cleanup..." << std::endl;
    std::cout << "Clearing GL Programs..." << std::endl;

    mazeRenderer->cleanup();

    std::cout << "Renderer Cleanup done." << std::endl;
}

void MainRenderManager::setMazeUsing(std::shared_ptr<Maze> maze) {
    this->maze = maze;
}

std::shared_ptr<Camera> MainRenderManager::getCamera() {
    return std::make_shared<PerspectiveCamera>(camera);
}

void MainRenderManager::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    this->width = width;
    this->height = height;
    camera.setScreenSize(width, height);
}

GLFWwindow* MainRenderManager::getWindow() {
    return window;
}

int MainRenderManager::getWidth() {
    return width;
}

int MainRenderManager::getHeight() {
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

    mazeRenderer->render();
}
#pragma endregion