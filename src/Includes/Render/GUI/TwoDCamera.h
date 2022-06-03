#pragma once
#include <Render/Camera.h>

class TwoDCamera : public Camera {
public:
	TwoDCamera(int screenWidth, int screenHeight);

	virtual float getXPos() override;
	virtual float getYPos() override;
	virtual float getZPos() override;

	void updateSizes(int newWidth, int newHeight);
};

