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
#include <Solvers/SimpleNeuralNetworkSolver.h>
#include <Solvers/DepthFirstSolver.h>
#include <Solvers/FloodFillSolver.h>
#include <Render/Camera.h>

#pragma once

std::shared_ptr<MainRenderManager> renderer;
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
    renderer->framebufferSizeCallback(window, width, height);
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
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        renderer->camera->rotateAround(
            renderer->camera->getXLookingAt(), 
            renderer->camera->getYLookingAt(), 
            renderer->camera->getZLookingAt(),
            -360.0f * camSpeed, 0.0f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        renderer->camera->rotateAround(
            renderer->camera->getXLookingAt(),
            renderer->camera->getYLookingAt(),
            renderer->camera->getZLookingAt(),
            360.0f * camSpeed, 0.0f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        renderer->camera->rotateAround(
            renderer->camera->getXLookingAt(),
            renderer->camera->getYLookingAt(),
            renderer->camera->getZLookingAt(),
            0.0f, -360.0f * camSpeed, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        renderer->camera->rotateAround(
            renderer->camera->getXLookingAt(),
            renderer->camera->getYLookingAt(),
            renderer->camera->getZLookingAt(),
            0.0f, 360.0f * camSpeed, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        renderer->camera->reset();
    }

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        renderer->camera->zoom(zoomSpeed);
    }

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        renderer->camera->zoom(-zoomSpeed);
    }

    if (glfwGetTime() > lastWShift + 0.2) {
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            renderer->mazeRenderer->setWViewing(renderer->mazeRenderer->getWViewing() - 1);
            lastWShift = glfwGetTime();
        }

        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            renderer->mazeRenderer->setWViewing(renderer->mazeRenderer->getWViewing() + 1);
            lastWShift = glfwGetTime();
        }
    }

    if (glfwGetTime() > lastPathShowChange + 0.2) {
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
            renderer->mazeRenderer->setShowPath(!renderer->mazeRenderer->showPath);
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
    if (renderer->getWindow() == NULL) {
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
    glViewport(0, 0, renderer->getWidth(), renderer->getHeight());

    //adjust viewport based on window resize (we set the callback here because C++ doesn't let you use member methods as callbacks)
    glfwSetFramebufferSizeCallback(renderer->getWindow(), framebufferSizeCallback);
    //set other callbacks
    std::cout << "Done." << std::endl;

    renderer->setup();

    //setup solvers thread

    std::cout << "Entering main loop..." << std::endl;
    double startDraw;
    //main render loop
    while (!glfwWindowShouldClose(renderer->getWindow())) {
        startDraw = glfwGetTime();

        renderer->render();
        glfwSwapBuffers(renderer->getWindow());

        delta = startDraw - lastFrame;
        lastFrame = startDraw;
        fps = 1 / delta;

        handleInput(renderer->getWindow());
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

    glfwSetWindowTitle(renderer->getWindow(), ("Maze Displayer and Solver - " + solverName).c_str());
}

void aiThreadMethod(std::shared_ptr<Maze> maze) {
    std::cout << "[AI] Initialising solvers..." << std::endl;
    SimpleNeuralNetworkSolver snnSolver({ 12, 10, 10, 4 }, 20, 0.1, maze, renderer);
    DepthFirstSolver depthSolver(maze, renderer);
    FloodFillSolver floodFillSolver(maze, renderer);

    std::vector<std::pair<std::string, std::shared_ptr<Solver>>> solversToNames = {
        {"Psuedo Right-Hand Rule Depth First Solver", std::make_shared<DepthFirstSolver>(depthSolver)},
        {"Flood Fill Solver", std::make_shared<FloodFillSolver>(floodFillSolver)}

    };

    std::cout << "[AI] Done." << std::endl;

    while (running) {
        //if (glfwGetTime() > lastSolverShift + 0.15) {
            if (solverIndex != lastSolverIndex) {
                //run next solver

                renderer->mazeRenderer->selectedPath.clearVisitedCells();
                

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

    //runSolver(std::make_unique<DepthFirstSolver>(depthSolver), maze, "Psuedo Right-Hand Rule Depth First Solver");

    //solver.solve();
}

int main() {
    Maze mazeObj;
    mazeObj.loadFromFile("maze.cd3mazs");

    std::shared_ptr<Maze> maze;

    std::cout << "Maze loaded" << std::endl;
    std::cout << "Maze entrance coords: " << mazeObj.mazeEntrance[0] << ", " << mazeObj.mazeEntrance[1] << ", " << mazeObj.mazeEntrance[2] << ", " << mazeObj.mazeEntrance[3] << std::endl;
    std::cout << "Maze exit coords: " << mazeObj.mazeExit[0] << ", " << mazeObj.mazeExit[1] << ", " << mazeObj.mazeExit[2] << ", " << mazeObj.mazeExit[3] << std::endl;

    maze = std::make_shared<Maze>(mazeObj);

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

    renderer = std::make_shared<MainRenderManager>(width, height/*800, 600*/, maze);

    std::thread aiThread(aiThreadMethod, maze);

    int result = beginRenderLoop(maze);

    aiThread.join();

	return result;
}