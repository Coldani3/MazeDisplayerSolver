#pragma once

struct CellPathTransformation {
public:
	float rotateAngleX, rotateAngleY, translateX, translateY, translateZ;

	CellPathTransformation(float rotateAngleX,
		float rotateAngleY,
		float translateX,
		float translateY,
		float translateZ) : rotateAngleX(rotateAngleX),
							rotateAngleY(rotateAngleY),
							translateX(translateX),
							translateY(translateY),
							translateZ(translateZ) {}
};