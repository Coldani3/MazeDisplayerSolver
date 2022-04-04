#pragma once
class Camera
{
private:
	float xRotation, yRotation, zRotation;
	float xPosition, yPosition, zPosition;
public:
	Camera(float xPos, float yPos, float zPos);
	Camera(float xPos, float yPos, float zPos, float xRotation, float yRotation, float zRotation);
	~Camera();
	/*using getters / setters in case I decide to do any funny business like store the positions in a vector to streamline
	  matrix code or rotations in a Quaternion for if I run into gimbal lock. possibly falls under YAGNI but sources on C++
	  conventions I find are... inconsistent. */
	float getXPos();
	float getYPos();
	float getZPos();
	float getXRotation();
	float getYRotation();
	float getZRotation();

	void setXPos(float);
	void setYPos(float);
	void setZPos(float);
	void setPos(float x, float y, float z);
	void setXRotation(float);
	void setYRotation(float);
	void setZRotation(float);
	void setRotation(float xRot, float yRot, float zRot);

	void rotate(float xRot, float yRot, float zRot);
	void translate(float xPos, float yPos, float zPos);
};

