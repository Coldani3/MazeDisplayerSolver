
#include <iostream>
#include <memory>
#include <chrono>
#include <vector>
#include <thread>
#include <map>

#include "Includes/MazeMain.h"

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
    if (threeDRenderer->window->getWindow() == NULL) {
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
    glfwSetFramebufferSizeCallback(threeDRenderer->window->getWindow(), framebufferSizeCallback);
    //set other callbacks
    std::cout << "Done." << std::endl;

    threeDRenderer->setup();
    guiRenderer->setup();

    //setup solvers thread

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

        handleInput(threeDRenderer->window->getWindow(), maze);
        glfwPollEvents();
    }

    std::cout << "Closing program..." << std::endl;
    glfwTerminate();
    running = false;
    return 1;
}

int main() {
    MazeMain main;
    return main.main();

    std::thread aiThread(aiThreadMethod, maze);

    int result = beginRenderLoop(maze);

    aiThread.join();

	return result;
}