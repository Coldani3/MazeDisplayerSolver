#ifndef __gl_h
#include <glad/glad.h>
#endif

#include <glfw3/glfw3.h>
#include <iostream>
#include <memory>
#include <chrono>
#include <vector>
#include <thread>
#include <map>

#include <Maze/Maze.h>
#include <Render/MainRenderManager.h>
#include <Render/GUI/GUIRenderManager.h>
#include <Solvers/SimpleNeuralNetworkSolver.h>
#include <Solvers/DepthFirstSolver.h>
#include <Solvers/FloodFillSolver.h>
#include <Render/PerspectiveCamera.h>
#include <Render/MazeRenderInfo.h>

#pragma once

//TODO: make these not global or namespace them or something
std::shared_ptr<MainRenderManager> threeDRenderer = nullptr;
std::shared_ptr<GUIRenderManager> guiRenderer = nullptr;

bool running = true;
double lastFrame;
double delta = 0;
float fps = 0;
double lastWShift = 0;
int solverIndex = 0;
int lastSolverIndex = -1;
double lastSolverShift = 0;

float camMoveSpeedMod = 1.0f;

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {    
    threeDRenderer->framebufferSizeCallback(window, width, height);
    guiRenderer->framebufferSizeCallback(window, width, height);
}

void handleInput(GLFWwindow* window, std::shared_ptr<Maze> maze) {    
    //TODO: programmatically access the renderers in a container of some sort and call them iteratively
    threeDRenderer->mazeRenderer->getRenderPollInput(window, delta);
    guiRenderer->fourDIndicator->getRenderPollInput(window, delta);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
        running = false;
    }

    //hmm ok so I had no idea what I was doing here, the fact it's stored in a variable itself is ownership
    //DRY, not using the smart pointers as we don't need to own it and the smart pointer already handles ownership
    //MazeRenderInfo *rendererInfo = threeDRenderer->mazeRenderInfo.get();

    std::shared_ptr<MazeRenderInfo> rendererInfo = threeDRenderer->mazeRenderInfo;

    //TODO: prevent these from being pressed during the transition OR skip the transition along.
    if (glfwGetTime() > lastWShift + 0.2 && glfwGetTime() > rendererInfo->wChangeAnimStart + rendererInfo->mazeTransitionAnimationSpeed) {
        bool fourDChangePressed = false;
        int w = rendererInfo->wViewing;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            w -= 1;
            fourDChangePressed = true;
        }

        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            w += 1;
            fourDChangePressed = true;
        }

        if (w >= 0 && w < maze->hyperDepth && fourDChangePressed) {
            //threeDRenderer->mazeRenderInfo->wViewing = w;
            rendererInfo->beginWTransitionAnim(w);//changeWViewingForAnims(w);
            lastWShift = glfwGetTime();
        }
    }

    if (glfwGetTime() > lastSolverShift + 1.0) {
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
            ++solverIndex;
            std::cout << "Incrementing Solver Index!" << std::endl;
            lastSolverShift = glfwGetTime();
        } else if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
            --solverIndex;
            std::cout << "Decrementing Solver Index!" << std::endl;
            lastSolverShift = glfwGetTime();
        }
    }
}

int beginRenderLoop(std::shared_ptr<Maze> maze) {
    if (threeDRenderer->getWindow() == NULL) {
        std::cerr << "Could not create window" << std::endl;
        glfwTerminate();
        return -1;
    }

    //load GLAD so it can do its thing with function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Could not initialise GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    //resize viewport
    glViewport(0, 0, threeDRenderer->getWidth(), threeDRenderer->getHeight());

    //adjust viewport based on window resize (we set the callback here because C++ doesn't let you use member methods as callbacks)
    glfwSetFramebufferSizeCallback(threeDRenderer->getWindow(), framebufferSizeCallback);
    //set other callbacks
    std::cout << "Done." << std::endl;

    threeDRenderer->setup();
    guiRenderer->setup();

    //setup solvers thread

    std::cout << "Entering main loop..." << std::endl;
    double startDraw;
    //main render loop
    while (!glfwWindowShouldClose(threeDRenderer->getWindow())) {
        startDraw = glfwGetTime();

        threeDRenderer->render();
        guiRenderer->render();
        glfwSwapBuffers(threeDRenderer->getWindow());

        delta = startDraw - lastFrame;
        lastFrame = startDraw;
        fps = 1.0f / delta;

        handleInput(threeDRenderer->getWindow(), maze);
        glfwPollEvents();
    }

    std::cout << "Closing program..." << std::endl;
    glfwTerminate();
    running = false;
    return 1;
}

long now() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void runSolver(std::shared_ptr<Solver> solver, std::shared_ptr<Maze> maze, std::string solverName) {
    std::cout << "[AI] Beginning " << solverName << " Solver..." << std::endl;

    long start = now();

    solver->solve();

    long finish = now();

    if (solver->success) {
        std::cout << "[AI] Success!";
    } else {
        std::cout << "[AI] Failed.";
    }

    std::cout << " Steps Taken: " << solver->stepsTaken << " in " << finish - start << " milliseconds " << std::endl;

    glfwSetWindowTitle(threeDRenderer->getWindow(), ("Maze Displayer and Solver - " + solverName).c_str());
}

//TODO: SolverManager class to handle this logic
void aiThreadMethod(std::shared_ptr<Maze> maze) {
    std::cout << "[AI] Initialising solvers..." << std::endl;
    SimpleNeuralNetworkSolver snnSolver({ 12, 10, 10, 4 }, 20, 0.1f, maze, threeDRenderer);
    DepthFirstSolver depthSolver(maze, threeDRenderer);
    FloodFillSolver floodFillSolver(maze, threeDRenderer);

    std::vector<std::pair<std::string, std::shared_ptr<Solver>>> solversToNames = {
        {"Psuedo Right-Hand Rule Depth First Solver", std::make_shared<DepthFirstSolver>(depthSolver)},
        {"Flood Fill Solver", std::make_shared<FloodFillSolver>(floodFillSolver)}

    };

    std::cout << "[AI] Done." << std::endl;

    while (running) {
        if (solverIndex != lastSolverIndex) {
            //run next solver

            threeDRenderer->mazeRenderer->selectedPath.clearVisitedCells();
                
            if (solverIndex >= 0 && solverIndex < solversToNames.size()) {
                solversToNames[solverIndex].second->clear();
                solversToNames[solverIndex].second->stepsTaken = 0;
                solversToNames[solverIndex].second->success = false;
                runSolver(solversToNames[solverIndex].second, maze, solversToNames[solverIndex].first);
            }

            lastSolverIndex = solverIndex;
        }
    }
}

int main() {
    //Mazes can get pretty big in memory so pointers are the call here.
    std::shared_ptr<Maze> maze = std::make_shared<Maze>();
    maze->loadFromFile("maze.cd3mazs");

    std::cout << "Maze loaded" << std::endl;
    std::cout << "Maze entrance coords: " << maze->mazeEntrance[0] << ", " << maze->mazeEntrance[1] << ", " << maze->mazeEntrance[2] << ", " << maze->mazeEntrance[3] << std::endl;
    std::cout << "Maze exit coords: " << maze->mazeExit[0] << ", " << maze->mazeExit[1] << ", " << maze->mazeExit[2] << ", " << maze->mazeExit[3] << std::endl;

    //initialise it here as renderer needs to be not null
    std::cout << "Initialising GLFW..." << std::endl;
    //initialise glfw
    glfwInit();

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

    int xpos, ypos, width, height;
    glfwGetMonitorWorkarea(monitor, &xpos, &ypos, &width, &height);
    std::shared_ptr<MazeRenderInfo> mazeRenderInfo = std::make_shared<MazeRenderInfo>(0);
    std::cout << "Initialising renderers..." << std::endl;
    threeDRenderer = std::make_shared<MainRenderManager>(width, height/*800, 600*/, maze);
    threeDRenderer->mazeRenderInfo = mazeRenderInfo;
    guiRenderer = std::make_shared<GUIRenderManager>(maze, width, height);
    guiRenderer->mazeRenderInfo = mazeRenderInfo;
    std::cout << "Done." << std::endl;

    std::thread aiThread(aiThreadMethod, maze);

    int result = beginRenderLoop(maze);

    aiThread.join();

	return result;
}