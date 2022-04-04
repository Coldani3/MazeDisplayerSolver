#include <glad/glad.h>
#include <glfw3/glfw3.h>
#include <iostream>
#include <memory>
#include <chrono>

#include <Maze/Maze.h>
#include <Render/RenderManager.h>

std::unique_ptr<RenderManager> renderer;
long fps = 0;

float scaleForFPS(float forEverySecond) {
    return forEverySecond / fps;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {    
    renderer->framebufferSizeCallback(window, width, height);
}

void handleInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        std::cout << "Rotate by " << scaleForFPS(360.0f) << std::endl;
        renderer->camera->rotate(scaleForFPS(360.0f), 0.0f, 0.0f);
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        std::cout << "Rotate by " << scaleForFPS(-360.0f) << std::endl;
        renderer->camera->rotate(scaleForFPS(-360.0f), 0.0f, 0.0f);
    }
}

int main() {
    std::cout << "Initialising GLFW..." << std::endl;
    //initialise glfw
    glfwInit();
    //tell glfw we're using opengl 3.3 with the core profile, instead of the old method
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    renderer = std::make_unique<RenderManager>(800, 600);

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
    double frameDuration;
    //main render loop
    while (!glfwWindowShouldClose(renderer->getWindow())) {
        startDraw = glfwGetTime();

        renderer->draw();
        glfwSwapBuffers(renderer->getWindow());

        frameDuration = glfwGetTime() - startDraw;

        std::cout << frameDuration << std::endl;

        fps = 1 / frameDuration;

        handleInput(renderer->getWindow());
        glfwPollEvents();
    }

    std::cout << "Closing program..." << std::endl;
    glfwTerminate();

	return 0;
}