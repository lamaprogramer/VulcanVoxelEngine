#pragma once

#include <tiny_obj_loader.h>

#include <filesystem>
#include <iostream> 
#include <string>
#include <map>
#include "Model.h"

class ModelManager {
public:
	std::map<std::string, Model> models;

	ModelManager();

	void addModel(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, std::string name, std::vector<Vertex> vertices, std::vector<uint32_t> indices);
	void loadModel(VulkanPhysicalDevice physicalDevice, VulkanLogicalDevice device, VulkanCommandPool commandPool, std::string modelPath);

	std::string getFileName(std::string path);
	Model& getModel(std::string name);

	void destroy(VulkanLogicalDevice device);
};