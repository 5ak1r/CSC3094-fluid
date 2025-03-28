#ifndef TEXTURE_H
#define TEXTURE_H

#include "glad.h"
#include "stb_image.h"

#include <iostream>
#include <string>

class Texture {
public:
    unsigned int ID;
    std::string path;
    std::string type;

    Texture(const char* tPath, std::string type);
    
    void use();

    void changeWrap(int sort, int type);
    void changeFilter(int minormag, int type);
};

#endif