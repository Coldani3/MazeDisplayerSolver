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

void MazeMain::checkWindowCloseKeyPressed(const Window& window) {
    if (glfwGetKey(window.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window.getWindow(), true);
        running = false;
    }
}

void MazeMain::handleFourDManoeuvre(const Window& window) {
    std::shared_ptr<MazeRenderInfo> rendererInfo = threeDRenderer->mazeRenderInfo;

    //TODO: prevent these from being pressed during the transition OR skip the transition along.
    if (glfwGetTime() > lastWShift + 0.2 && glfwGetTime() > rendererInfo->wChangeAnimStart + rendererInfo->mazeTransitionAnimationSpeed) {
        bool fourDChangePressed = false;
        int w = rendererInfo->wViewing;
        if (glfwGetKey(window.getWindow(), GLFW_KEY_Q) == GLFW_PRESS) {
            w -= 1;
            fourDChangePressed = true;
        }

        if (glfwGetKey(window.getWindow(), GLFW_KEY_E) == GLFW_PRESS) {
            w += 1;
            fourDChangePressed = true;
        }

        if (w >= 0 && w < maze->hyperDepth && fourDChangePressed) {
            //threeDRenderer->mazeRenderInfo->wViewing = w;
            rendererInfo->beginWTransitionAnim(w);//changeWViewingForAnims(w);
            lastWShift = glfwGetTime();
        }
    }
}

void MazeMain::handleSolverIndexControls(const Window& window) {
    if (glfwGetTime() > lastSolverShift + 1.0) {
        if (glfwGetKey(window.getWindow(), GLFW_KEY_L) == GLFW_PRESS) {
            aiManager->changeSolverIndex(1);

            std::cout << "Incrementing Solver Index!" << std::endl;

            lastSolverShift = glfwGetTime();
        } else if (glfwGetKey(window.getWindow(), GLFW_KEY_K) == GLFW_PRESS) {
            aiManager->changeSolverIndex(-1);

            std::cout << "Decrementing Solver Index!" << std::endl;

            lastSolverShift = glfwGetTime();
        }
    }
}

void MazeMain::handleRendererInput(const Window& window, std::shared_ptr<Maze> maze) {
    GLFWwindow* windowPtr = window.getWindow();
    threeDRenderer->mazeRenderer->getRenderPollInput(windowPtr, delta);
    guiRenderer->fourDIndicator->getRenderPollInput(windowPtr, delta);
}

void MazeMain::handleInput(const Window& window, std::shared_ptr<Maze> maze) {
    handleRendererInput(window, maze);
    checkWindowCloseKeyPressed(window);

    handleFourDManoeuvre(window);
    handleSolverIndexControls(window);
}

void MazeMain::setupFramebufferCallback(const Window& window) {
    glfwSetWindowUserPointer(window.getWindow(), (void*)this);
    glfwSetFramebufferSizeCallback(threeDRenderer->window->getWindow(), [](GLFWwindow* window, int width, int height) {
        MazeMain* main = (MazeMain*) glfwGetWindowUserPointer(window);
        main->framebufferSizeCallback(window, width, height); 
    });
}

#pragma endregion

#pragma region Main Execution
void MazeMain::renderLoop() {
    std::cout << "Entering main loop..." << std::endl;
    double startDraw;
    //main render loop
    while (!glfwWindowShouldClose(threeDRenderer->window->getWindow())) {
        startDraw = glfwGetTime();

        threeDRenderer->render();
        guiRenderer->render();
        glfwSwapBuffers(threeDRenderer->window->getWindow());

        delta = startDraw - lastFrame;
        lastFrame = startDraw;
        fps = 1.0f / delta;

        handleInput(*window, maze);
        glfwPollEvents();
    }
}
#pragma endregion