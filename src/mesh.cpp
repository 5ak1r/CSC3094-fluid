#include "../include/mesh.hpp"

#include <cstddef>
#include <string>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
: vertices(std::move(vertices)), indices(std::move(indices)), textures(std::move(textures)) {
    setupMesh();
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, Material noTextures)
: vertices(std::move(vertices)), indices(std::move(indices)), textures(std::move(textures)), noTextures(noTextures) {
    setupMesh();
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &this->indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}

void Mesh::draw(Shader &shader) const {
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;

    for(unsigned int i = 0; i < this->textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);

        std::string number;
        std::string name = this->textures[i].type;

        if(name == "texture_diffuse") number = std::to_string(diffuseNr++);
        else if(name == "texture_specular") number = std::to_string(specularNr++);

        shader.setInt((name + number).c_str(), i);
        
        glBindTexture(GL_TEXTURE_2D, this->textures[i].ID);
    }

    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(this->indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}

void Mesh::drawInstanced(Shader &shader, const std::vector<glm::mat4> &modelMatrices) {
    glBindVertexArray(this->VAO);

    if (this->instanceVBO == 0) {
        glGenBuffers(1, &this->instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, this->instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, modelMatrices.size() * sizeof(glm::mat4), &modelMatrices[0], GL_DYNAMIC_DRAW);

        for(unsigned int j = 0; j < 4; j++) {
            glEnableVertexAttribArray(3 + j);
            glVertexAttribPointer(3 + j, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * j));
            glVertexAttribDivisor(3 + j, 1);
        }
    } else {
        glBindBuffer(GL_ARRAY_BUFFER, this->instanceVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, modelMatrices.size() * sizeof(glm::mat4), &modelMatrices[0]);
    }

    if(textures.size() > 0) {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;

        for(unsigned int i = 0; i < this->textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);

            std::string number;
            std::string name = this->textures[i].type;

            if(name == "texture_diffuse") number = std::to_string(diffuseNr++);
            else if(name == "texture_specular") number = std::to_string(specularNr++);

            shader.setInt((name + number).c_str(), i);
            
            glBindTexture(GL_TEXTURE_2D, this->textures[i].ID);
        }
    } else {
        shader.setVec3("material.ambient", noTextures.ambient);
        shader.setVec3("material.diffuse", noTextures.diffuse);
        shader.setVec3("material.specular", noTextures.specular);
        shader.setFloat("material.shininess", noTextures.shininess);

        shader.setVec3("light.position", glm::vec3(1.0f, 1.0f, 1.0f));
        shader.setVec3("light.ambient",  glm::vec3(0.1f));
        shader.setVec3("light.diffuse",  glm::vec3(1.0f));
        shader.setVec3("light.specular", glm::vec3(1.0f));
    }

    glDrawElementsInstanced(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0, modelMatrices.size());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}