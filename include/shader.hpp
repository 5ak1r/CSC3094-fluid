#ifndef SHADER_H
#define SHADER_H

#include "glad.h"
#include "glm/glm.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>


class Shader {
public:
    unsigned int ID;
    
    Shader(const char* vPath, const char* fPath);

    void use();
    void del();

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

    void setFloat3(const std::string &name, float x, float y, float z) const;
    void setFloat3(const std::string &name, glm::vec3 vec) const;
    void setFloat4(const std::string &name, float r, float g, float b, float a) const;
    void setFloat4(const std::string &name, glm::vec3 vec, float w) const;
    void setFloat4(const std::string &name, glm::vec4) const;

    void setMatrix1(const std::string &name, const float* value_ptr) const;
};

#endif