#pragma once
#include <glad/glad.h>
#include <glfw3/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
	int screenWidth, screenHeight;
	float xPosition = 0;
	float yPosition = 0;
	float zPosition = 0;

	Camera();
	Camera(int width, int height);
	virtual glm::mat4 getView();
	virtual glm::mat4 getProjection();

	//return 0,0,0 if camera has no position
	virtual float getXPos();
	virtual float getYPos();
	virtual float getZPos();

	virtual void updateProjection();

protected:
	glm::mat4 projection;
};

