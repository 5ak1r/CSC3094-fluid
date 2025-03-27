#ifndef MESH_H
#define MESH_H

#include "glm/glm.hpp"
#include "shader.hpp"
#include "texture.hpp"

#include <vector>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 texCoords;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

    void Draw(Shader &shader);

private:
    unsigned int VAO, VBO, EBO;

    void setupMesh();

};

#endif