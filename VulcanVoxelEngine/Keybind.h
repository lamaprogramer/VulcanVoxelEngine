#pragma once
#include <functional>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Keybind {
public:
	Keybind();

	Keybind(int keyBind, bool sticky);

	void update(GLFWwindow* window, std::function<void()> call);

	int get();

private:
	int keyBind;
	bool sticky = false;
	bool canUpdate = true;
};