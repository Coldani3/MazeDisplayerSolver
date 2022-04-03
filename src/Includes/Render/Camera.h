#pragma once
class Camera
{
private:
	float xRotation, yRotation, zRotation;
	int xPos, yPos, zPos;
public:
	Camera(int xPos, int yPos, int zPos);
	Camera(int xPos, int yPos, int zPos, float xRotation, float yRotation, float zRotation);
	~Camera();
	/*using getters / setters in case I decide to do any funny business like store the positions in a vector to streamline
	  matrix code or rotations in a Quaternion for if I run into gimbal lock. possibly falls under YAGNI but sources on C++
	  conventions I find are... inconsistent. */
	int getXPos();
	int getYPos();
	int getZPos();
	float getXRotation();
	float getYRotation();
	float getZRotation();

	void setXPos(int);
	void setYPos(int);
	void setZPos(int);
	void setPos(int x, int y, int z);
	void setXRotation(float);
	void setYRotation(float);
	void setZRotation(float);
	void setRotation(float xRot, float yRot, float zRot);

};

