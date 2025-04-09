#ifndef MESH_H
#define MESH_H

#include "glm/glm.hpp"
#include "shader.hpp"
#include "texture.hpp"

#include <vector>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    
    float shininess;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Material noTextures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, Material noTextures);

    void draw(Shader &shader) const;
    void drawInstanced(Shader &shader, const std::vector<glm::mat4> &modelMatrices);

private:
    unsigned int VAO, VBO, EBO;
    unsigned int instanceVBO = 0;

    void setupMesh();

};

#endif