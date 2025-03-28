#include "../include/texture.hpp"
#include <ostream>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include "../include/stb_image.h"

#endif

Texture::Texture(const char* tPath, std::string type) {
    this->path = tPath;
    this->type = type;
    
    glGenTextures(1, &this->ID);
    glBindTexture(GL_TEXTURE_2D, this->ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(tPath, &width, &height, &nrChannels, 0);

    std::string str(tPath);

    if (data) {

        GLenum format;
        switch(nrChannels) {
            case 1:
                format = GL_RED;
                break;
            case 3:
                format = GL_RGB;
                break;
            case 4:
                format = GL_RGBA;
                break;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "ERROR::TEXTURE::FAILED_LOAD" << std::endl;
    }

    stbi_image_free(data);
}

void Texture::use() {
    glBindTexture(GL_TEXTURE_2D, this->ID);
}

void Texture::changeWrap(int sort, int type) {
    if ((sort == GL_TEXTURE_WRAP_S || sort == GL_TEXTURE_WRAP_T)
        && (type == GL_REPEAT || type == GL_CLAMP_TO_BORDER
        || type == GL_CLAMP_TO_EDGE || type == GL_MIRRORED_REPEAT)) {
            glBindTexture(GL_TEXTURE_2D, this->ID);
            glTexParameteri(GL_TEXTURE_2D, sort, type);
        }
    else {
        std::cout << "ERROR::TEXTURE::CHANGE_WRAP::INCORRECT_INPUT" << std::endl;
    }
        
}

void Texture::changeFilter(int minormag, int type) {
    if ((minormag == GL_TEXTURE_MIN_FILTER || minormag == GL_TEXTURE_MAG_FILTER)
        && (type == GL_NEAREST || type == GL_LINEAR)) {
            glBindTexture(GL_TEXTURE_2D, this->ID);
            glTexParameteri(GL_TEXTURE_2D, minormag, type);
        }
    else {
        std::cout << "ERROR::TEXTURE::CHANGE_FILTER::INCORRECT_INPUT" << std::endl;
    }

}