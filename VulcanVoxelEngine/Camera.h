#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "CameraKeybinds.h"

#include <glm/gtc/matrix_transform.hpp> 
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

class Camera {
public:
	Camera();
	Camera(GLFWwindow* window, glm::vec3 position, float centerX, float centerY);

	void update(GLFWwindow* window);
	glm::mat4 getViewMatrix();
	glm::vec3 getPosition();
	glm::vec3 getFront();

private:
	CameraKeybinds keybinds;
	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f;
	float lastX, lastY;
	bool firstMouse;

	glm::vec3 position;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp = glm::vec3(0.0, 1.0, 0.0);

	float pitch = 0.0f;
	float yaw = -90.0f;
};