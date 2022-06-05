#include <Render/GUI/TwoDCamera.h>

TwoDCamera::TwoDCamera(int screenWidth, int screenHeight) {
    projection = glm::ortho(0.0f, static_cast<float>(screenWidth), static_cast<float>(screenHeight), 0.0f, -1.0f, 1.0f);
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
}

float TwoDCamera::getXPos() {
    return 0.0f;
}

float TwoDCamera::getYPos() {
    return 0.0f;
}

float TwoDCamera::getZPos() {
    return 0.0f;
}

void TwoDCamera::updateSizes(int newWidth, int newHeight) {
    screenWidth = newWidth;
    screenHeight = newHeight;
    projection = glm::ortho(0.0f, static_cast<float>(newWidth), static_cast<float>(newHeight), 0.0f, -1.0f, 1.0f);
}
