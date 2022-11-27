#include "Window.h"

Window::Window(int width, int height, std::string title) {
    //create the window
    window = glfwCreateWindow(width, height, title.c_str()/*"Maze Displayer and Solver"*/, NULL, NULL);
    //set to top left corner

    int left, top, right, bottom;
    glfwGetWindowFrameSize(window, &left, &top, &right, &bottom);

    glfwSetWindowSize(window, width, height - top);
    glfwSetWindowPos(window, 0, top);

    glfwMakeContextCurrent(window);

    defaultWidth = width;
    defaultHeight = height - top;
    this->width = defaultWidth;
    this->height = defaultHeight;
}

void Window::setWindowTitle(std::string title) {
    glfwSetWindowTitle(window, title.c_str());
}

bool Window::shouldClose() {
    return glfwWindowShouldClose(window);
}

void Window::swapBuffers() {
    glfwSwapBuffers(window);
}

GLFWwindow* Window::getWindow() {
    return window;
}
