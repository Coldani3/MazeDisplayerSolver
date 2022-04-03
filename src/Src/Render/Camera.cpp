#include <Render/Camera.h>

Camera::Camera(int xPos, int yPos, int zPos) : Camera(xPos, yPos, zPos, 0.0f, 0.0f, 0.0f) {
}

Camera::Camera(int xPos, int yPos, int zPos, float xRotation, float yRotation, float zRotation) {
	this->xPos = xPos;
	this->yPos = yPos;
	this->zPos = zPos;
	this->xRotation = xRotation;
	this->yRotation = yRotation;
	this->zRotation = zRotation;
}

Camera::~Camera() {
}

int Camera::getXPos() {
	return xPos;
}

int Camera::getYPos() {
	return yPos;
}

int Camera::getZPos() {
	return zPos;
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

void Camera::setXPos(int x) {
	xPos = x;
}

void Camera::setYPos(int y) {
	yPos = y;
}

void Camera::setZPos(int z) {
	zPos = z;
}

void Camera::setPos(int x, int y, int z) {
	xPos = x;
	yPos = y;
	zPos = z;
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
