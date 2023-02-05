#include "MazeMain.h"


int MazeMain::main() {
    loadMaze();
    setupGLFW();
    GLFWmonitor* monitor = setupMonitor();

    getScreenInfoForWindow(monitor, windowWidth, windowHeight);

    window = std::make_shared<Window>(windowWidth, windowHeight, "Maze Displayer and Solver");
    inputManager = std::make_shared<InputManager>(window);

    bool terminate = false;

    if (!window->initialised()) {
        std::cerr << "Failed to load GLFW!" << '\n';
        terminate = true;
    }

    if (!initialiseGLAD()) {
        std::cerr << "Failed to initialise GLAD!" << '\n';
        terminate = true;
    }

    if (terminate) {
        glfwTerminate();
        return -1;
    }

    mazePathManager = std::make_shared<MazePathManager>();

    setupRenderers();

    //Add a default path to avoid wonkiness with empty paths.
    MazePath defaultPath(maze->width, maze->height, maze->depth, maze->hyperDepth);
    defaultPath.markCellVisited(maze->mazeEntrance);
    mazePathManager->setActivePath(defaultPath);

    aiManager = std::make_shared<AIManager>(window, mazePathManager);//std::make_unique<AIManager>(maze, running);

    setupViewport();
    setupFramebufferCallback(*window);

    threeDRenderer->setup();
    guiRenderer->setup();

    std::thread aiThread([this]() {
        this->aiManager->run(this->maze, this->running);
    });

    renderLoop();

    glfwTerminate();
    running = false;

    aiThread.join();
    return 1;
}

#pragma region Setup
void MazeMain::loadMaze() {
    //Mazes can be several kilos of memory on their own - if not stored as pointers they may end up being inadvertently 
    //copied and eating memory
    maze = std::make_shared<Maze>();//loadMazeFromFile("maze.cd3mazs");
    maze->loadFromFile("maze.cd3mazs");

    std::cout << "Maze loaded" << std::endl;
    std::cout << "Maze entrance coords: " << maze->mazeEntrance.x() << ", " << maze->mazeEntrance.y() << ", " << maze->mazeEntrance.z() << ", " << maze->mazeEntrance.w() << std::endl;
    std::cout << "Maze exit coords: " << maze->mazeExit.x() << ", " << maze->mazeExit.y() << ", " << maze->mazeExit.z() << ", " << maze->mazeExit.w() << std::endl;
}

void MazeMain::setupGLFW() noexcept {
    std::cout << "Initialising GLFW..." << std::endl;
    glfwInit();
}

GLFWmonitor* MazeMain::setupMonitor() noexcept {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    //use opengl 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    return monitor;
}

void MazeMain::getScreenInfoForWindow(GLFWmonitor* monitor, int& windowWidth, int& windowHeight) noexcept {
    glfwGetMonitorWorkarea(monitor, &windowXPos, &windowYPos, &windowWidth, &windowHeight);
}

void MazeMain::setupRenderers() noexcept {
    std::shared_ptr<MazeRenderInfo> mazeRenderInfo = std::make_shared<MazeRenderInfo>(0);

    std::cout << "Initialising renderers..." << std::endl;

    threeDRenderer = std::make_shared<MainRenderManager>(window, maze, mazePathManager);
    threeDRenderer->mazeRenderInfo = mazeRenderInfo;

    guiRenderer = std::make_shared<GUIRenderManager>(maze, windowWidth, windowHeight);
    guiRenderer->mazeRenderInfo = mazeRenderInfo;

    std::cout << "Done." << std::endl;
}

bool MazeMain::initialiseGLAD() {
    return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 1;
}

void MazeMain::setupViewport() {
    glViewport(0, 0, threeDRenderer->getWidth(), threeDRenderer->getHeight());
}

void MazeMain::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    threeDRenderer->framebufferSizeCallback(window, width, height);
    guiRenderer->framebufferSizeCallback(window, width, height);
}

void MazeMain::checkWindowCloseKeyPressed(const Window& window) {
    if (inputManager->getKeyPressed(GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window.getWindow(), true);
        window.setShouldClose();
        running = false;
    }
}

void MazeMain::handleFourDManoeuvre(const Window& window) {
    std::shared_ptr<MazeRenderInfo> rendererInfo = threeDRenderer->mazeRenderInfo;

    //TODO: prevent these from being pressed during the transition OR skip the transition along.
    if (glfwGetTime() > lastWShift + 0.2 && glfwGetTime() > rendererInfo->wChangeAnimStart + rendererInfo->mazeTransitionAnimationSpeed) {
        bool fourDChangePressed = false;
        int& w = rendererInfo->wViewing;

        if (inputManager->getKeyPressed(GLFW_KEY_Q)) {
            w -= 1;
            fourDChangePressed = true;
        }

        if (inputManager->getKeyPressed(GLFW_KEY_E)) {
            w += 1;
            fourDChangePressed = true;
        }

        if (maze->wInBounds(w) && fourDChangePressed) {
            //threeDRenderer->mazeRenderInfo->wViewing = w;
            rendererInfo->beginWTransitionAnim(w);//changeWViewingForAnims(w);
            lastWShift = glfwGetTime();
        }
    }
}

void MazeMain::handleSolverIndexControls(const Window& window) {
    if (glfwGetTime() > lastSolverShift + 1.0) {
        if (inputManager->getKeyPressed(GLFW_KEY_L)) {
            aiManager->changeSolverIndex(1);

            std::cout << "Incrementing Solver Index!" << std::endl;

            lastSolverShift = glfwGetTime();
        } else if (inputManager->getKeyPressed(GLFW_KEY_K)) {
            aiManager->changeSolverIndex(-1);

            std::cout << "Decrementing Solver Index!" << std::endl;

            lastSolverShift = glfwGetTime();
        }
    }
}

void MazeMain::handleRendererInput(const Window& window, std::shared_ptr<Maze> maze) {
    GLFWwindow* windowPtr = window.getWindow();
    threeDRenderer->mazeRenderer->getRenderPollInput(windowPtr, delta, *inputManager);
    guiRenderer->fourDIndicator->getRenderPollInput(windowPtr, delta, *inputManager);
}

void MazeMain::handleInput(const Window& window, std::shared_ptr<Maze> maze) {
    handleRendererInput(window, maze);
    checkWindowCloseKeyPressed(window);

    handleFourDManoeuvre(window);
    handleSolverIndexControls(window);
}

void MazeMain::setupFramebufferCallback(const Window& window) {
    glfwSetWindowUserPointer(window.getWindow(), reinterpret_cast<void*>(this));
    glfwSetFramebufferSizeCallback(window.getWindow(), [](GLFWwindow* window, int width, int height) {
        MazeMain* main = reinterpret_cast<MazeMain*>(glfwGetWindowUserPointer(window));
        main->framebufferSizeCallback(window, width, height); 
    });
}

#pragma endregion

#pragma region Main Execution
void MazeMain::renderLoop() {
    std::cout << "Entering main loop..." << std::endl;
    double startDraw;
    //main render loop
    while (!window->shouldClose()) {
        startDraw = glfwGetTime();

        threeDRenderer->render();
        guiRenderer->render();
        glfwSwapBuffers(window->getWindow());

        delta = startDraw - lastFrame;
        lastFrame = startDraw;
        fps = 1.0f / delta;

        handleInput(*window, maze);
        glfwPollEvents();
    }
}
#pragma endregion