#pragma once
class Camera
{
private:
	float xPosition, yPosition, zPosition;
	float xLookingAt, yLookingAt, zLookingAt;
public:
	Camera(float xPos, float yPos, float zPos);
	Camera(float xPos, float yPos, float zPos, float xLookingAt, float yLookingAt, float zLookingAt);
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

	void setXPos(float);
	void setYPos(float);
	void setZPos(float);
	void moveTo(float x, float y, float z);
	void setXRotation(float);
	void setYRotation(float);
	void setZRotation(float);
	void setRotation(float xRot, float yRot, float zRot);

	void rotate(float xRot, float yRot, float zRot);
	void moveBy(float xPos, float yPos, float zPos);
	void lookAt(float xPos, float yPos, float zPos);
	void rotateAround(float xPos, float yPos, float zPos, float xRot, float yRot, float zRot);
	//Where magnitude is how far along in GL units you move along the path between you and what the camera is set to look at
	void zoom(float magnitude);
};

