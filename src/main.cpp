#include <glad/glad.h>
#include <glfw3/glfw3.h>
#include <iostream>

#include <Maze/Maze.h>
#include <Render/RenderManager.h>

RenderManager renderer;

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {    
    renderer.framebufferSizeCallback(window, width, height);
}

void handleInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main() {
    //initialise glfw
    glfwInit();
    //tell glfw we're using opengl 3.3 with the core profile, instead of the old method
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    renderer = RenderManager(800, 600);

    if (renderer.getWindow() == NULL) {
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
    glViewport(0, 0, renderer.getWidth(), renderer.getHeight());

    //adjust viewport based on window resize (we set the callback here because C++ doesn't let you use member methods as callbacks)
    glfwSetFramebufferSizeCallback(renderer.getWindow(), framebufferSizeCallback);
    //set other callbacks

    renderer.setup();

    //setup solvers thread

    //main render loop
    while (!glfwWindowShouldClose(renderer.getWindow())) {
        handleInput(renderer.getWindow());

        renderer.draw();

        glfwSwapBuffers(renderer.getWindow());
        glfwPollEvents();
    }

    glfwTerminate();

	return 0;
}