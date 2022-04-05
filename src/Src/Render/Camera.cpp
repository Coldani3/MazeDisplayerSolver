#include <Render/Camera.h>
#include <cmath>

Camera::Camera(float xPos, float yPos, float zPos) : Camera(xPos, yPos, zPos, 0.0f, 0.0f, 0.0f) {
}

Camera::Camera(float xPos, float yPos, float zPos, float xRotation, float xLookingAt, float yLookingAt, float zLookingAt) {
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
}

void Camera::rotateAround(float xPos, float yPos, float zPos, float xRot, float yRot, float zRot) {
	//record distance from the spot so we rotate around it in a sphere
	float distance = sqrt(pow(xPosition - xPos, 2) + pow(yPosition - yPos, 2) + pow(zPosition - zPos, 2));
	//YXZ to reduce gimbal lock

	//the circle rotates around 0,0 by default so translate them by the new positions to put it in the right place
	float newX = sin(360 / xRot) * distance;
	float newY = cos(360 / yRot) * distance;
}

