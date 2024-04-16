#include "Physics.h"

glm::vec3 Physics::update(float deltaTime) {
	lastVelocity = velocity;
	lastAcceleration = acceleration;

	acceleration = (velocity - lastVelocity) / deltaTime;
	return velocity * deltaTime;
}

glm::vec3 Physics::deltaVelocity() {
	return velocity - lastVelocity;
}