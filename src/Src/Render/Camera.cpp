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

glm::vec3 Camera::getCoords() {
	return glm::vec3(getXPos(), getYPos(), getZPos());
}

void Camera::updateProjection() {
}
