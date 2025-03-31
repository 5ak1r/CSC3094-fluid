#ifndef PARTICLE_H
#define PARTICLE_H

#include "glm/fwd.hpp"
#include "glm/glm.hpp"

struct Particle {
    glm::mat4 model;

    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 force;

    float density;
    float pressure;

    Particle(glm::vec3 position, glm::mat4 model) : position(position), model(model) {
        this->velocity = glm::vec3(0.0f);
        this->acceleration = glm::vec3(0.0f, -9.81f, 0.0f);
        this->force = glm::vec3(0.0f);
        this->density = 0.0f;
        this->pressure = 0.0f;
    }
    void updatePhysics(float deltaTime) {
        this->velocity = this->velocity + this->acceleration * deltaTime;
    }
};

#endif