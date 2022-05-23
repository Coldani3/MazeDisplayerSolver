#include <Render/Camera.h>
#include <cmath>
#include <iostream>

Camera::Camera(float xPos, float yPos, float zPos, int screenWidth, int screenHeight) : Camera(xPos, yPos, zPos, screenWidth, screenHeight, 0.0f, 0.0f, 0.0f) {
}

Camera::Camera(float xPos, float yPos, float zPos, int screenWidth, int screenHeight, float xLookingAt, float yLookingAt, float zLookingAt) {
	moveTo(xPos, yPos, zPos);
	/*xPosition = xPos;
	yPosition = yPos;
	zPosition = zPos;*/
	lookAt(xLookingAt, yLookingAt, zLookingAt);
	/*this->xLookingAt = xLookingAt;
	this->yLookingAt = yLookingAt;
	this->zLookingAt = zLookingAt;*/
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	defaultXLookingAt = xLookingAt;
	defaultYLookingAt = yLookingAt;
	defaultZLookingAt = zLookingAt;
	defaultXPosition = xPos;
	defaultYPosition = yPos;
	defaultZPosition = zPos;

	updateProjection();
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

glm::mat4 Camera::getProjection() {
	return projection;
}

glm::mat4 Camera::getViewMatrix() {
	glm::vec3 camPos = glm::vec3(xPosition, yPosition, zPosition);
	glm::vec3 lookingAtPos = glm::vec3(xLookingAt, yLookingAt, zLookingAt);

	glm::mat4 lookAtMat = glm::lookAt(camPos, lookingAtPos, glm::vec3(0.0f, 1.0f, 0.0f));

	return lookAtMat;
}

void Camera::setXPos(float x) {
	moveTo(x, yPosition, zPosition);
}

void Camera::setYPos(float y) {
	moveTo(xPosition, y, zPosition);
}

void Camera::setZPos(float z) {
	moveTo(xPosition, yPosition, z);
}

void Camera::updateRotation() {
	glm::vec3 lookingAt = glm::vec3(xLookingAt, yLookingAt, zLookingAt);
	glm::vec3 camPos = glm::vec3(xPosition, yPosition, zPosition);

	glm::vec3 hypotenuse = lookingAt - camPos;

	//toa - tan^-1(yDistance / xDistance) - atan(yDistance / xDistance)
	//we dont' care about roll here
	//TODO: what about 0 z or x difference
	pitch = atan(hypotenuse.y / hypotenuse.x);
	yaw = atan(hypotenuse.x / hypotenuse.z);
}

void Camera::setCameraMode(CameraMode newMode) {
	camMode = newMode;
}

void Camera::setRotation(float pitch, float yaw, float roll) {
	this->pitch = pitch;
	this->yaw = yaw;
	this->roll = roll;
}

void Camera::rotateBy(float pitchBy, float yawBy, float rollBy) {
	setCameraMode(CameraMode::FREE_CAM);
	pitch += pitchBy;
	yaw += yawBy;
	roll += rollBy;
	setRotation(pitch, yaw, roll);

	//update lookingAt to be the front of the camera

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	direction = glm::normalize(direction);

	xLookingAt = direction.x;
	yLookingAt = direction.y;
	zLookingAt = direction.z;
}

void Camera::moveTo(float x, float y, float z) {
	xPosition = x;
	yPosition = y;
	zPosition = z;
}

void Camera::setRenderDistanceMin(float min) {
	minRenderDistance = min;
	updateProjection();
}

void Camera::setRenderDistanceMax(float max) {
	maxRenderDistance = max;
	updateProjection();
}

void Camera::setFOV(float angle) {
	fov = angle;
	updateProjection();
}

void Camera::moveBy(float x, float y, float z) {
	moveTo(xPosition + x, yPosition + y, zPosition + z);
}

//TODO: For rotating freely instead of around a point, create a vector point in front of the camera which is rotated around
//the camera that the camera continuously looks at.
//or actually just use lookingAt as that
void Camera::lookAt(float xPos, float yPos, float zPos) {
	xLookingAt = xPos;
	yLookingAt = yPos;
	zLookingAt = zPos;
}

void Camera::rotateAround(float xPos, float yPos, float zPos, float xRot, float yRot, float zRot) {
	setCameraMode(CameraMode::ROTATE_AROUND);
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
	//translate to origin, rotateBy, then translate back
	glm::vec3 out = translateBack * rotate * translateToOrigin * glm::vec4(camVec, 1.0);

	moveTo(out.x, out.y, out.z);

	lookAt(xPos, yPos, zPos);
	updateRotation();
	/*xLookingAt = xPos;
	yLookingAt = yPos;
	zLookingAt = zPos;*/

	//std::cout << xPosition << "," << yPosition << "," << zPosition << std::endl;
}

void Camera::zoom(float magnitude) {
	glm::vec3 camCoords = glm::vec3(xPosition, yPosition, zPosition);
	glm::vec3 lookingAtCoords = glm::vec3(xLookingAt, yLookingAt, zLookingAt);
	glm::vec3 path = camCoords - lookingAtCoords;


	if (camMode == CameraMode::ROTATE_AROUND) {
		float lerp = magnitude / glm::length(path);

		glm::vec3 newCamPos = lerp * lookingAtCoords + (1 - lerp) * camCoords;

		xPosition = newCamPos.x;
		yPosition = newCamPos.y;
		zPosition = newCamPos.z;
	} else if (camMode == CameraMode::FREE_CAM) {
		//get direction facing
		glm::vec3 unitPath = glm::normalize(path);
		//then traverse that by magnitude times
		glm::vec3 newCamPos = camCoords + (unitPath * magnitude);

		xPosition = newCamPos.x;
		yPosition = newCamPos.y;
		zPosition = newCamPos.z;
	}
}

void Camera::reset() {
	moveTo(defaultXPosition, defaultYPosition, defaultZPosition);
	lookAt(defaultXLookingAt, defaultYLookingAt, defaultZLookingAt);
}

void Camera::updateProjection() {
	projection = glm::perspective(glm::radians(fov), (float)screenWidth / (float)screenHeight, minRenderDistance, maxRenderDistance);
}
