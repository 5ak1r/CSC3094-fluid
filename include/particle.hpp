#ifndef PARTICLE_H
#define PARTICLE_H

#include "glm/fwd.hpp"
#include "glm/glm.hpp"

#include <vector>
#include <unordered_map>

class Particle {
public:
    glm::mat4 model;

    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 force;

    glm::ivec3 cell;
    uint hash;

    float density;
    float pressure;

    std::vector<Particle*> neighbours;

    Particle(glm::vec3 position, glm::mat4 model);

    void calcCell();
    void calcHash();

    uint cellHash(const glm::ivec3 &cell);

    glm::mat4 updatePhysics(float deltaTime);

    static void sortParticles(std::vector<Particle*> particles);
    static std::unordered_map<uint32_t, uint32_t> neighbourTable(std::vector<Particle*> sortedParticles);

    void getNeighbours(std::vector<Particle*> sortedParticles, std::unordered_map<uint32_t, uint32_t>& neighbourTable);
};

#endif