#include <glad/glad.h>
#include <glfw3/glfw3.h>
#include <iostream>
#include <memory>
#include <chrono>
#include <vector>
#include <thread>

#include <Maze/Maze.h>
#include <Render/RenderManager.h>
#include <Solvers/SimpleNeuralNetworkSolver.h>

std::unique_ptr<RenderManager> renderer;
double lastFrame;
double delta = 0;
int fps = 0;
double lastWShift = 0;

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
            renderer->setWViewing(renderer->getWViewing() + 1);
            lastWShift = glfwGetTime();
        }

        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            renderer->setWViewing(renderer->getWViewing() - 1);
            lastWShift = glfwGetTime();
        }
    }
}

int beginRenderLoop(Maze maze) {
    std::cout << "Initialising GLFW..." << std::endl;
    //initialise glfw
    glfwInit();
    //tell glfw we're using opengl 3.3 with the core profile, instead of the old method
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    renderer = std::make_unique<RenderManager>(800, 600, maze);

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

        renderer->draw();
        glfwSwapBuffers(renderer->getWindow());

        delta = startDraw - lastFrame;
        lastFrame = startDraw;
        fps = 1 / delta;

        handleInput(renderer->getWindow());
        glfwPollEvents();
    }

    std::cout << "Closing program..." << std::endl;
    glfwTerminate();
}

void aiThreadMethod(Maze maze) {
    SimpleNeuralNetworkSolver solver({ 12, 10, 10, 4 }, 20, 0.1, maze);

    //solver.solve();
}

int main() {
    
    Maze maze;
    maze.loadFromFile("maze.cd3mazs");

    std::cout << "Maze loaded" << std::endl;
    std::cout << "Maze entrance coords: " << maze.mazeEntrance[0] << ", " << maze.mazeEntrance[1] << ", " << maze.mazeEntrance[2] << ", " << maze.mazeEntrance[3] << std::endl;
    std::cout << "Maze exit coords: " << maze.mazeExit[0] << ", " << maze.mazeExit[1] << ", " << maze.mazeExit[2] << ", " << maze.mazeExit[3] << std::endl;
    std::cout << (unsigned int) maze[{0, 0, 0, 0}] << std::endl;

    std::thread aiThread(aiThreadMethod, maze);

    int result = beginRenderLoop(maze);

    aiThread.join();

	return result;
}