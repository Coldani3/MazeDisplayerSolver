#include <Render/RenderManager.h>
#include <Render/MazeRenderer.h>

#pragma region Class Methods
RenderManager::RenderManager(int width, int height, Maze maze) {
    //create the window
    window = glfwCreateWindow(width, height, "Maze Displayer and Solver", NULL, NULL);
    //set to top left corner

    int left, top, right, bottom;
    glfwGetWindowFrameSize(window, &left, &top, &right, &bottom);

    glfwSetWindowSize(window, width, height - top);
    glfwSetWindowPos(window, 0, top);

    glfwMakeContextCurrent(window);

    defaultWidth = width;
    defaultHeight = height;
    this->width = width;
    this->height = height;
    this->maze = maze;

    std::vector<float> mazeCenter = { centerX + ((float)maze.width / 2), centerY + ((float)maze.height / 2), centerZ + ((float)maze.depth / 2) };

    camera = std::make_shared<Camera>(mazeCenter[0], mazeCenter[1], mazeCenter[2] + -15.0f, defaultWidth, defaultHeight, mazeCenter[0], mazeCenter[1], mazeCenter[2]);
    camera->lookAt(mazeCenter[0], mazeCenter[1], mazeCenter[2]);

    mazeRenderer = std::make_unique<MazeRenderer>(maze, centerX, centerY, centerZ);
    std::cout << "t" << std::endl;
    mazeRenderer->setCamera(camera);

    projection = glm::perspective(glm::radians(45.0f), (float) this->width / (float) this->height, 0.1f, 100.0f);
}

RenderManager::~RenderManager() {
    std::cout << "Beginning Renderer Cleanup..." << std::endl;
    std::cout << "Clearing GL Programs..." << std::endl;

    mazeRenderer->cleanup();

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

void RenderManager::setup() {
    std::cout << "Setting up OpenGL..." << std::endl;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    //glEnable(GL_CULL_FACE);

    mazeRenderer->setup();

    std::cout << "Finished setting up OpenGL." << std::endl;
}

void RenderManager::draw() {
    glClearColor(0.8470f, 0.8823f, 0.9098f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mazeRenderer->render();

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