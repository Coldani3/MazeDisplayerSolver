#include <Render/Camera.h>
#include <cmath>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float xPos, float yPos, float zPos) : Camera(xPos, yPos, zPos, 0.0f, 0.0f, 0.0f) {
}

Camera::Camera(float xPos, float yPos, float zPos, float xLookingAt, float yLookingAt, float zLookingAt) {
	this->xPosition = xPos;
	this->yPosition = yPos;
	this->zPosition = zPos;
	this->xLookingAt = xLookingAt;
	this->yLookingAt = yLookingAt;
	this->zLookingAt = zLookingAt;
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
	float xDifference = xPosition - xPos;
	float yDifference = yPosition - yPos;
	float zDifference = zPosition - zPos;

	xLookingAt = xPos;
	yLookingAt = yPos;
	zLookingAt = zPos;
}

void Camera::rotateAround(float xPos, float yPos, float zPos, float xRot, float yRot, float zRot) {
	glm::mat4 identity = glm::mat4(1.0);
	glm::vec3 aroundVec = glm::vec3(xPos, yPos, zPos);
	glm::vec3 camVec = glm::vec3(xPosition, yPosition, zPosition);

	glm::mat4 translateToOrigin = glm::translate(identity, glm::vec3(-xPos, -yPos, -zPos));
	glm::mat4 translateBack = glm::translate(identity, aroundVec);
	glm::mat4 rotateY = glm::rotate(identity, glm::radians(yRot), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 rotateX = glm::rotate(identity, glm::radians(xRot), glm::vec3(1.0, 0.0, 0.0));
	glm::mat4 rotateZ = glm::rotate(identity, glm::radians(zRot), glm::vec3(0.0, 0.0, 1.0));

	//YXZ to reduce gimbal lock
	glm::mat4 rotate = rotateZ * rotateX * rotateY;
	//translate to origin, rotate, then translate back
	glm::vec3 out = translateBack * rotate * translateToOrigin * glm::vec4(camVec, 1.0);

	xPosition = out.x;
	yPosition = out.y;
	zPosition = out.z;

	xLookingAt = xPos;
	yLookingAt = yPos;
	zLookingAt = zPos;


	//rotate around the sphere

	//float newXChange, newYChange, newZChange;

	//newXChange = sin(xRot);
	//newZChange = cos(xRot);

	//float newX = normalizedX + ((cos(xRot) + cos(yRot)) * distance);
	//float newY = normalizedY + (sin(yRot) * distance);
	//float newZ = normalizedZ + ((sin(xRot) + cos(yRot)) * distance);

	//xLookingAt = xPos;
	//yLookingAt = yPos;
	//zLookingAt = zPos;

	////translate back to its original coordinates
	//xPosition = newX + xPos;
	//yPosition = newY + yPos;
	//zPosition = newZ + zPos;
	//std::cout << distance << std::endl;
}

