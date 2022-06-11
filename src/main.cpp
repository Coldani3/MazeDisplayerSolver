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

#pragma once

//TODO: make these not global or namespace them or something
std::shared_ptr<MainRenderManager> threeDRenderer = nullptr;
std::shared_ptr<GUIRenderManager> guiRenderer = nullptr;
bool running = true;
double lastFrame;
double delta = 0;
int fps = 0;
double lastWShift = 0;
double lastPathShowChange = 0;
int solverIndex = 0;
int lastSolverIndex = -1;
double lastSolverShift = 0;

float camMoveSpeedMod = 1.0f;

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {    
    threeDRenderer->framebufferSizeCallback(window, width, height);
    guiRenderer->framebufferSizeCallback(window, width, height);
}

void handleInput(GLFWwindow* window) {
    float camSpeed = 0.1 * delta;
    float zoomSpeed = 2.5 * delta;

    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        camMoveSpeedMod += 1.0 * delta;
    }

    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
        camMoveSpeedMod -= 1.0 * delta;
    }

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
        camMoveSpeedMod = 1.0;
    }

    /*camSpeed += camMoveSpeedMod;
    zoomSpeed += camMoveSpeedMod;*/
    

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
        running = false;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        threeDRenderer->camera->rotateAround(
            threeDRenderer->camera->getXLookingAt(), 
            threeDRenderer->camera->getYLookingAt(),
            threeDRenderer->camera->getZLookingAt(),
            -360.0f * camSpeed, 0.0f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        threeDRenderer->camera->rotateAround(
            threeDRenderer->camera->getXLookingAt(),
            threeDRenderer->camera->getYLookingAt(),
            threeDRenderer->camera->getZLookingAt(),
            360.0f * camSpeed, 0.0f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        threeDRenderer->camera->rotateAround(
            threeDRenderer->camera->getXLookingAt(),
            threeDRenderer->camera->getYLookingAt(),
            threeDRenderer->camera->getZLookingAt(),
            0.0f, -360.0f * camSpeed, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        threeDRenderer->camera->rotateAround(
            threeDRenderer->camera->getXLookingAt(),
            threeDRenderer->camera->getYLookingAt(),
            threeDRenderer->camera->getZLookingAt(),
            0.0f, 360.0f * camSpeed, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        threeDRenderer->camera->reset();
    }

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        threeDRenderer->camera->zoom(zoomSpeed);
    }

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        threeDRenderer->camera->zoom(-zoomSpeed);
    }

    if (glfwGetTime() > lastWShift + 0.2) {
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            int w = threeDRenderer->mazeRenderer->getWViewing() - 1;
            threeDRenderer->mazeRenderer->setWViewing(w);
            guiRenderer->fourDIndicator->setWViewing(w);
            lastWShift = glfwGetTime();
        }

        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            int w = threeDRenderer->mazeRenderer->getWViewing() + 1;
            threeDRenderer->mazeRenderer->setWViewing(w);
            guiRenderer->fourDIndicator->setWViewing(w);
            lastWShift = glfwGetTime();
        }
    }

    if (glfwGetTime() > lastPathShowChange + 0.2) {
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
            threeDRenderer->mazeRenderer->setShowPath(!threeDRenderer->mazeRenderer->showPath);
            lastPathShowChange = glfwGetTime();
        }
    }

    if (glfwGetTime() > lastSolverShift + 1.0) {
        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
            ++solverIndex;
            std::cout << "Incrementing Solver Index!" << std::endl;
            lastSolverShift = glfwGetTime();
        } else if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
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
        fps = 1 / delta;

        handleInput(threeDRenderer->getWindow());
        glfwPollEvents();
    }

    std::cout << "Closing program..." << std::endl;
    glfwTerminate();
    running = false;
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
    SimpleNeuralNetworkSolver snnSolver({ 12, 10, 10, 4 }, 20, 0.1, maze, threeDRenderer);
    DepthFirstSolver depthSolver(maze, threeDRenderer);
    FloodFillSolver floodFillSolver(maze, threeDRenderer);

    std::vector<std::pair<std::string, std::shared_ptr<Solver>>> solversToNames = {
        {"Psuedo Right-Hand Rule Depth First Solver", std::make_shared<DepthFirstSolver>(depthSolver)},
        {"Flood Fill Solver", std::make_shared<FloodFillSolver>(floodFillSolver)}

    };

    std::cout << "[AI] Done." << std::endl;

    while (running) {
        //if (glfwGetTime() > lastSolverShift + 0.15) {
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
        //}
    }
}

int main() {
    Maze mazeObj;
    mazeObj.loadFromFile("maze.cd3mazs");

    std::cout << "Maze loaded" << std::endl;
    std::cout << "Maze entrance coords: " << mazeObj.mazeEntrance[0] << ", " << mazeObj.mazeEntrance[1] << ", " << mazeObj.mazeEntrance[2] << ", " << mazeObj.mazeEntrance[3] << std::endl;
    std::cout << "Maze exit coords: " << mazeObj.mazeExit[0] << ", " << mazeObj.mazeExit[1] << ", " << mazeObj.mazeExit[2] << ", " << mazeObj.mazeExit[3] << std::endl;

    //Mazes can get pretty big in memory so pointers are the call here.
    std::shared_ptr<Maze> maze = std::make_shared<Maze>(mazeObj);

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

    std::cout << "Initialising renderers..." << std::endl;
    threeDRenderer = std::make_shared<MainRenderManager>(width, height/*800, 600*/, maze);
    guiRenderer = std::make_shared<GUIRenderManager>(maze, width, height);
    std::cout << "Done." << std::endl;

    std::thread aiThread(aiThreadMethod, maze);

    int result = beginRenderLoop(maze);

    aiThread.join();

	return result;
}