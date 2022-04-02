#pragma once
class Camera
{
private:
	int xRotation, yRotation, zRotation;
	int xPos, yPos, zPos;
public:
	/*using getters / setters in case I decide to do any funny business like store the positions in a vector to streamline
	  matrix code or rotations in a Quaternion for if I run into gimbal lock. possibly falls under YAGNI but sources on C++
	  conventions I find are... inconsistent. */
	int getXPos();
	int getYPos();
	int getZPos();
	int getXRotation();
	int getYRotation();
	int getZRotation();

	void setXPos(int);
	void setYPos(int);
	void setZPos(int);
	void setPos(int x, int y, int z);
	void setXRotation(int);
	void setYRotation(int);
	void setZRotation(int);
	int setRotation(int xRot, int yRot, int zRot);

};

