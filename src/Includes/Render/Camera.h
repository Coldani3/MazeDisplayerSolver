#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
private:
	float xPosition, yPosition, zPosition;
	float xLookingAt, yLookingAt, zLookingAt;
	//roll unused
	float pitch = 0;
	float yaw = -90;
	float roll = 0;
	float fov = 45.0f;
	float minRenderDistance = 0.1f;
	float maxRenderDistance = 100.0f;
	float defaultXLookingAt, defaultYLookingAt, defaultZLookingAt;
	float defaultXPosition, defaultYPosition, defaultZPosition;
	int screenWidth, screenHeight;
	glm::mat4 projection;

public:
	Camera(float xPos, float yPos, float zPos, int screenWidth, int screenHeight);
	Camera(float xPos, float yPos, float zPos, int screenWidth, int screenHeight, float xLookingAt, float yLookingAt, float zLookingAt);
	~Camera();
	/*using getters / setters in case I decide to do any funny business like store the positions in a vector to streamline
	  matrix code or rotations in a Quaternion for if I run into gimbal lock. possibly falls under YAGNI but sources on C++
	  conventions I find are... inconsistent. */
	float getXPos();
	float getYPos();
	float getZPos();
	float getXLookingAt();
	float getYLookingAt();
	float getZLookingAt();

	glm::mat4 getProjection();
	void setRenderDistanceMin(float);
	void setRenderDistanceMax(float);
	void setFOV(float angle);

	void setXPos(float);
	void setYPos(float);
	void setZPos(float);
	void setPitch(float);
	void setYaw(float);
	void setRoll(float);

	void setRotation(float pitch, float yaw, float roll);
	void rotateBy(float pitch, float yaw, float roll);
	void moveTo(float x, float y, float z);
	void moveBy(float xPos, float yPos, float zPos);
	void lookAt(float xPos, float yPos, float zPos);
	void rotateAround(float xPos, float yPos, float zPos, float xRot, float yRot, float zRot);
	//Where magnitude is how far along in GL units you move along the path between you and what the camera is set to look at
	void zoom(float magnitude);
	void reset();

private:
	void updateProjection();
};

