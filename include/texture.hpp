#ifndef TEXTURE_H
#define TEXTURE_H

#include "glad.h"
#include "stb_image.h"

#include <iostream>
#include <string>

class Texture {
public:
    unsigned int ID;
    const char* type;

    Texture(const char* tPath, const char* type);
    
    void use();

    void changeWrap(int sort, int type);
    void changeFilter(int minormag, int type);
};

#endif