#include "../include/shader.hpp"
#include <fstream>

Shader::Shader(const char* vPath, const char* fPath) {
    std::string vCode;
    std::string fCode;

    std::ifstream vsFile;
    std::ifstream fsFile;

    vsFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fsFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vsFile.open(vPath);
        fsFile.open(fPath);

        std::stringstream vsStream, fsStream;

        vsStream << vsFile.rdbuf();
        fsStream << fsFile.rdbuf();

        vsFile.close();
        fsFile.close();

        vCode = vsStream.str();
        fCode = fsStream.str();   
    } catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_READ: " << e.what() << std::endl;
    }

    const char* vsCode = vCode.c_str();
    const char* fsCode = fCode.c_str();

    unsigned int vertex, fragment;

    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vsCode, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fsCode, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    this->ID = glCreateProgram();

    glAttachShader(this->ID, vertex);
    glAttachShader(this->ID, fragment);

    glLinkProgram(this->ID);

    glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
    
    if (!success) {
        glGetProgramInfoLog(this->ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() {
    glUseProgram(this->ID);
}

void Shader::del() {
    glDeleteProgram(this->ID);
}

GLint Shader::getUniformLocation(const std::string &name) const {
    GLint location = glGetUniformLocation(this->ID, name.c_str());
    
    if (location == -1 && name != "texture_specular1") {
        std::cerr << "ERROR::SHADER::UNIFORM::NOT_FOUND::" << name << std::endl;
        std::exit(EXIT_FAILURE);
    }

    return location;
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(this->getUniformLocation(name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(this->getUniformLocation(name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(this->getUniformLocation(name.c_str()), value);
}

void Shader::setFloat3(const std::string &name, float x, float y, float z) const {
    glUniform3f(this->getUniformLocation(name.c_str()), x, y, z);
}

void Shader::setFloat4(const std::string &name, float r, float g, float b, float a) const {
    glUniform4f(this->getUniformLocation(name.c_str()), r, g, b, a);
}

void Shader::setFloat4(const std::string &name, glm::vec3 vec, float w) const {
    glUniform4f(this->getUniformLocation(name.c_str()), vec.x, vec.y, vec.z, w);
}

void Shader::setFloat4(const std::string &name, glm::vec4 vec) const {
    glUniform4f(this->getUniformLocation(name.c_str()), vec.x, vec.y, vec.z, vec.w);
}

void Shader::setVec3(const std::string &name, glm::vec3 vec) const {
    glUniform3f(this->getUniformLocation(name.c_str()), vec.x, vec.y, vec.z);
}

void Shader::setMatrix1(const std::string &name, const float* value_ptr) const {
    glUniformMatrix4fv(this->getUniformLocation(name.c_str()), 1, GL_FALSE, value_ptr);
}