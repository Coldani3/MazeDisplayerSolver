#include <Render/Camera.h>
#include <cmath>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float xPos, float yPos, float zPos) : Camera(xPos, yPos, zPos, 0.0f, 0.0f, 0.0f) {
}

Camera::Camera(float xPos, float yPos, float zPos, float xLookingAt, float yLookingAt, float zLookingAt) {
	xPosition = xPos;
	yPosition = yPos;
	zPosition = zPos;
	this->xLookingAt = xLookingAt;
	this->yLookingAt = yLookingAt;
	this->zLookingAt = zLookingAt;

	defaultXLookingAt = xLookingAt;
	defaultYLookingAt = yLookingAt;
	defaultZLookingAt = zLookingAt;
	defaultXPosition = xPos;
	defaultYPosition = yPos;
	defaultZPosition = zPos;
}

Camera::~Camera() {
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

float Camera::getXLookingAt() {
	return xLookingAt;
}

float Camera::getYLookingAt() {
	return yLookingAt;
}

float Camera::getZLookingAt() {
	return zLookingAt;
}

void Camera::setXPos(float x) {
	xPosition = x;
}

void Camera::setYPos(float y) {
	yPosition = y;
}

void Camera::setZPos(float z) {
	zPosition = z;
}

void Camera::moveTo(float x, float y, float z) {
	xPosition = x;
	yPosition = y;
	zPosition = z;
}

void Camera::moveBy(float x, float y, float z) {
	moveTo(xPosition + x, yPosition + y, zPosition + z);
}

void Camera::lookAt(float xPos, float yPos, float zPos) {
	xLookingAt = xPos;
	yLookingAt = yPos;
	zLookingAt = zPos;
}

void Camera::rotateAround(float xPos, float yPos, float zPos, float xRot, float yRot, float zRot) {
	glm::mat4 identity = glm::mat4(1.0);
	glm::vec3 aroundVec = glm::vec3(xPos, yPos, zPos);
	glm::vec3 camVec = glm::vec3(xPosition, yPosition, zPosition);

	glm::mat4 translateToOrigin = glm::translate(identity, -aroundVec);
	glm::mat4 translateBack = glm::translate(identity, aroundVec);
	//remember that rotating on an axis does not mean rotating around the center of that plane -
	//it rotates around the axis itself like a spiral staircase
	glm::mat4 rotateY = glm::rotate(identity, glm::radians(yRot), glm::vec3(1.0, 0.0, 0.0));
	glm::mat4 rotateX = glm::rotate(identity, glm::radians(xRot), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 rotateZ = glm::rotate(identity, glm::radians(zRot), glm::vec3(0.0, 0.0, 1.0));

	//YXZ to reduce gimbal lock
	glm::mat4 rotate = rotateZ * rotateX * rotateY;
	//translate to origin, rotate, then translate back
	glm::vec3 out = translateBack * rotate * translateToOrigin * glm::vec4(camVec, 1.0);

	/*while (glm::dot(glm::normalize(out), glm::normalize(aroundVec)) > 0.9) {
		out = glm::vec4(out, 1.0) * glm::rotate(identity, glm::radians(1.0f), glm::vec3(0.0, 1.0, 0.0));
	}*/

	xPosition = out.x;
	yPosition = out.y;
	zPosition = out.z;

	xLookingAt = xPos;
	yLookingAt = yPos;
	zLookingAt = zPos;

	//std::cout << xPosition << "," << yPosition << "," << zPosition << std::endl;
}

void Camera::zoom(float magnitude) {
	glm::vec3 camCoords = glm::vec3(xPosition, yPosition, zPosition);
	glm::vec3 lookingAtCoords = glm::vec3(xLookingAt, yLookingAt, zLookingAt);
	glm::vec3 path = camCoords - lookingAtCoords;
	float lerp = magnitude / glm::length(path);

	glm::vec3 newCamPos = lerp * lookingAtCoords + (1 - lerp) * camCoords;

	xPosition = newCamPos.x;
	yPosition = newCamPos.y;
	zPosition = newCamPos.z;
}

void Camera::reset() {
	moveTo(defaultXPosition, defaultYPosition, defaultZPosition);
	lookAt(defaultXLookingAt, defaultYLookingAt, defaultZLookingAt);
}