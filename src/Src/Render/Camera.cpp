#include <Render/Camera.h>
#include <cmath>
#include <iostream>
#include <glm/glm.hpp>

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
	//translate cam coords to relative to 0 0 0 so we can treat it as a sphere around 0 0 0 then translate them back again later
	float normalizedX = xPosition - xPos;
	float normalizedY = yPosition - yPos;
	float normalizedZ = zPosition - zPos;


	//record distance from the spot so we rotate around it in a sphere
	float distance = sqrt(pow(normalizedX, 2) + pow(normalizedY, 2) + pow(normalizedZ, 2));
	//YXZ to reduce gimbal lock

	//rotate around the sphere
	float newX = normalizedX + (sin(xRot) * distance);
	float newY = normalizedY + (sin(yRot) * distance);
	float newZ = normalizedZ + (cos(zRot) * distance);

	xLookingAt = xPos;
	yLookingAt = yPos;
	zLookingAt = zPos;

	//translate back to its original coordinates
	xPosition = newX + xPos;
	yPosition = newY + yPos;
	zPosition = newZ + zPos;
	std::cout << distance << std::endl;
}

