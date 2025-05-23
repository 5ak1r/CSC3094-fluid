#ifndef MODEL_H
#define MODEL_H

#include "glm/glm.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "texture.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>

class Model {
public:
    Model(std::string path);
    
    void draw(Shader &shader) const;
    void drawInstanced(Shader &shader, const std::vector<glm::mat4> &modelMatrices);

private:
    std::vector<Texture> textures_loaded;
    std::vector<Mesh> meshes;
    std::string directory;

    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);

    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
    Material loadMaterial(aiMaterial* mat);
};

#endif