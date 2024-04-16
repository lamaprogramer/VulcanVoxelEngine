#pragma once
#include <glm/glm.hpp>

class Physics {
public:
	glm::vec3 velocity;
	glm::vec3 acceleration;

	glm::vec3 lastVelocity;
	glm::vec3 lastAcceleration;
	glm::vec3 update(float deltaTime);

private:
	glm::vec3 deltaVelocity();
};