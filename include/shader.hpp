#ifndef SHADER_H
#define SHADER_H

#include "glad.h"

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

    void setFloat4(const std::string &name, float r, float g, float b, float a) const;
};

#endif