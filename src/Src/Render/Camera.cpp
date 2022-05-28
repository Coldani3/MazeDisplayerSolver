#include <Render/Camera.h>

#include <iostream>

Camera::Camera() {
}

Camera::Camera(int width, int height) {
	screenWidth = width;
	screenHeight = height;
}

glm::mat4 Camera::getView() {
	return glm::mat4(1.0f);
}

glm::mat4 Camera::getProjection() {
	return projection;
}

float Camera::getXPos() {
	return xPosition;
}

float Camera::getYPos() {
	return yPosition;
}

float Camera::getZPos() {
	return zPosition;
}

void Camera::updateProjection() {
}
