#include <Render/Camera.h>

int Camera::getXPos() {
	return xPos;
}

int Camera::getYPos() {
	return yPos;
}

int Camera::getZPos() {
	return zPos;
}

int Camera::getXRotation() {
	return xRotation;
}

int Camera::getYRotation() {
	return yRotation;
}

int Camera::getZRotation() {
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

void Camera::setXRotation(int xRot) {
	xRotation = xRot;
}

void Camera::setYRotation(int yRot) {
	yRotation = yRot;
}

void Camera::setZRotation(int zRot) {
	zRotation = zRot;
}

int Camera::setRotation(int xRot, int yRot, int zRot) {
	xRotation = xRot;
	yRotation = yRot;
	zRotation = zRot;
}
