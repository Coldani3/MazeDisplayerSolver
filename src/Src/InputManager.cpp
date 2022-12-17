#include "InputManager.h"

InputManager::InputManager(std::shared_ptr<Window> window) {
    this->window = window;
}

int InputManager::getKey(int key) const noexcept {
    return glfwGetKey(window->getWindow(), key);
}

bool InputManager::getKeyPressed(int key) const noexcept {
    return getKey(key) == GLFW_PRESS;
}
