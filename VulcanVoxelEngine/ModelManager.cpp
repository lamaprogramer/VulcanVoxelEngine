#include "ModelManager.h"

ModelManager::ModelManager() {

}

void ModelManager::addModel(std::string name, std::vector<Vertex> vertices, std::vector<uint32_t> indices) {
	Model model = Model{ vertices, indices };
	models[name] = model;
}

void ModelManager::loadModel(std::string modelPath) {
    Model model = Model{};

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, modelPath.c_str())) {
        throw std::runtime_error(err);
    }

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            Vertex vertex{};

            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                attrib.texcoords[2 * index.texcoord_index + 1]
            };

            vertex.color = { 1.0f, 1.0f, 1.0f };

            model.vertices.push_back(vertex);
            model.indices.push_back(model.indices.size());
        }
    }

    models[getFileName(modelPath)] = model;
}

std::string ModelManager::getFileName(std::string path) {
    std::string base_filename = path.substr(path.find_last_of("/\\") + 1);

    std::string::size_type const p(base_filename.find_last_of('.'));
    return base_filename.substr(0, p);
}

Model& ModelManager::getModel(std::string name) {
	return models[name];
}
