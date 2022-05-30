#pragma once
#include <Render/Camera.h>

class TwoDCamera : public Camera {
public:
	TwoDCamera();

	virtual float getXPos() override;
	virtual float getYPos() override;
	virtual float getZPos() override;
};

