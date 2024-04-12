#include "Physics.h"

glm::vec3 Physics::update(float deltaTime) {
	lastVelocity = velocity;

	return velocity * deltaTime;
}

glm::vec3 Physics::deltaVelocity() {
	return velocity - lastVelocity;
}