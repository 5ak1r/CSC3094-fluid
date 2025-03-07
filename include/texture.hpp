#ifndef TEXTURE_H
#define TEXTURE_H

#include "glad.h"
#include "stb_image.h"

#include <iostream>
#include <string>

class Texture {
public:
    unsigned int ID;

    Texture(const char* tPath);


};

#endif