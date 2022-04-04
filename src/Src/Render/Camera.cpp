#include <Render/Camera.h>

Camera::Camera(float xPos, float yPos, float zPos) : Camera(xPos, yPos, zPos, 0.0f, 0.0f, 0.0f) {
}

Camera::Camera(float xPos, float yPos, float zPos, float xRotation, float yRotation, float zRotation) {
	this->xPosition = xPos;
	this->yPosition = yPos;
	this->zPosition = zPos;
	this->xRotation = xRotation;
	this->yRotation = yRotation;
	this->zRotation = zRotation;
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

float Camera::getXRotation() {
	return xRotation;
}

float Camera::getYRotation() {
	return yRotation;
}

float Camera::getZRotation() {
	return zRotation;
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

void Camera::setPos(float x, float y, float z) {
	xPosition = x;
	yPosition = y;
	zPosition = z;
}

void Camera::setXRotation(float xRot) {
	xRotation = xRot;
}

void Camera::setYRotation(float yRot) {
	yRotation = yRot;
}

void Camera::setZRotation(float zRot) {
	zRotation = zRot;
}

void Camera::setRotation(float xRot, float yRot, float zRot) {
	xRotation = xRot;
	yRotation = yRot;
	zRotation = zRot;
}

void Camera::rotate(float x, float y, float z) {
	setRotation(xRotation + x, yRotation + y, zRotation + z);
}

void Camera::translate(float x, float y, float z) {
	setPos(xPosition + x, yPosition + y, zPosition + z);
}

