#ifndef CD3_RENDERER_CAMERA
#define CD3_RENDERER_CAMERA
#pragma once
#include <glad/glad.h>
#include <glfw3/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
	int screenWidth, screenHeight;
	float camSpeed = 0.1f;
	float zoomSpeed = 2.5f;

	Camera();
	Camera(int width, int height);
	virtual glm::mat4 getView();
	virtual glm::mat4 getProjection();
	

	//return 0,0,0 if camera has no position
	virtual float getXPos() = 0;
	virtual float getYPos() = 0;
	virtual float getZPos() = 0;

	virtual glm::vec3 getCoords();

	virtual void updateProjection();

protected:
	glm::mat4 projection;
};

#endif