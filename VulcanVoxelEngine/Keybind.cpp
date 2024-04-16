#include "Keybind.h"


Keybind::Keybind() {
	this->keyBind = 0;
	this->sticky = false;
}

Keybind::Keybind(int keyBind, bool sticky) {
	this->keyBind = keyBind;
	this->sticky = sticky;
}

void Keybind::update(GLFWwindow* window, std::function<void()> call) {
	if (canUpdate) {
		if (glfwGetKey(window, this->keyBind) == GLFW_PRESS) {
			if (sticky) this->canUpdate = false;
			call();
		}
	}
	if (sticky) {
		if (glfwGetKey(window, this->keyBind) == GLFW_RELEASE) {
			this->canUpdate = true;
		}
	}
}

int Keybind::get() {
	return this->keyBind;
}