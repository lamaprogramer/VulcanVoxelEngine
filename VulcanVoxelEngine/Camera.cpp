#include "Camera.h"
#include <iostream>

Camera::Camera() {
	Camera(nullptr, glm::vec3(0, 0, 2), 0, 0);
}
Camera::Camera(GLFWwindow* window, glm::vec3 position, float centerX, float centerY) {
	this->position = position;

	CameraKeybinds bindings{};
	bindings.forward = GLFW_KEY_W;
	bindings.backward = GLFW_KEY_S;
	bindings.left = GLFW_KEY_A;
	bindings.right = GLFW_KEY_D;
	bindings.up = GLFW_KEY_SPACE;
	bindings.down = GLFW_KEY_LEFT_SHIFT;

	this->keybinds = bindings;
	lastX = centerX;
	lastY = centerY;
}

void Camera::update(GLFWwindow* window, float deltaTime) {
	float cameraSpeed = 10.5f * deltaTime;

	if (glfwGetKey(window, keybinds.forward) == GLFW_PRESS) {
		position += cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, keybinds.backward) == GLFW_PRESS) {
		position -= cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, keybinds.left) == GLFW_PRESS) {
		position -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, keybinds.right) == GLFW_PRESS) {
		position += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, keybinds.up) == GLFW_PRESS) {
		position += cameraSpeed * cameraUp;
	}
	if (glfwGetKey(window, keybinds.down) == GLFW_PRESS) {
		position -= cameraSpeed * cameraUp;
	}

	double cursorX;
	double cursorY;
	glfwGetCursorPos(window, &cursorX, &cursorY);

	if (firstMouse)
	{
		lastX = cursorX;
		lastY = cursorY;
		firstMouse = false;
	}

	float xoffset = cursorX - lastX;
	float yoffset = lastY - cursorY; // reversed since y-coordinates range from bottom to top
	lastX = cursorX;
	lastY = cursorY;

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f) // constraints
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}

glm::vec3 Camera::getPosition() {
	return position;
}
glm::vec3 Camera::getFront() {
	return cameraFront;
}
glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(
		position,
		position + cameraFront,
		cameraUp
	);
}