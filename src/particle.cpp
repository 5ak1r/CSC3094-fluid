#include "../include/particle.hpp"
#include "../include/constants.hpp"

#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"

#include <algorithm>
#include <cstdlib>
#include <vector>
#include <unordered_map>
#include <unordered_set>

Particle::Particle(glm::vec3 position, glm::mat4 model) : position(position), model(model) {
    this->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    this->acceleration = glm::vec3(0.0f, GRAVITY, 0.0f);
    this->force = glm::vec3(0.0f);

    this->calcCell();
    this->calcHash();

    this->density = 0.0f;
    this->pressure = 0.0f;
}

void Particle::calcCell() {
    
    this->cell = this->position / TRANSLATE;
}

void Particle::calcHash() {
    this->hash = this->cellHash(this->cell);
}

uint Particle::cellHash(const glm::ivec3 &cell) {
    return ((uint)(cell.x * 73856093) ^
            (uint)(cell.y * 19349663) ^
            (uint)(cell.z * 83492791) % PARTICLE_COUNT);
}

glm::mat4 Particle::updatePhysics(float deltaTime) {

    this->velocity += this->acceleration * deltaTime;
    this->position += this->velocity * deltaTime;
    
    if(this->position.y <= 0.0f) {
        this->position.y = 0.0f;
        this->velocity.y *= -0.9f;

        if(glm::abs(this->velocity.y) < 0.1f) this->velocity.y = 0.0f;
    }

    if (std::abs(this->position.x) >= BOX_SIZE) {
        this->position.x = (this->position.x > 0.0f) ? BOX_SIZE : -BOX_SIZE;
        this->velocity.x *= -0.9f;
    }

    if (std::abs(this->position.z) >= BOX_SIZE) {
        this->position.z = (this->position.z > 0.0f) ? BOX_SIZE : -BOX_SIZE;
        this->velocity.z *= -0.9f;
    }

    this->model = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(SCALE)), this->position);
    this->calcCell();
    this->calcHash();

    return this->model;
}

void Particle::sortParticles(std::vector<Particle*> particles) {
    std::sort(particles.begin(), particles.end(), [](const Particle* i, const Particle* j) { return i->hash < j->hash; });
}

std::unordered_map<uint32_t, uint32_t> Particle::neighbourTable(std::vector<Particle*> sortedParticles) {
    std::unordered_map<uint32_t, uint32_t> neighbourTable;

    for (size_t i = 0; i < PARTICLE_COUNT; i++) {
        neighbourTable[sortedParticles[i]->hash] = i;
    }

    return neighbourTable;
}

void Particle::getNeighbours(std::vector<Particle*> sortedParticles, std::unordered_map<uint32_t, uint32_t> &neighbourTable) {
    std::unordered_set<Particle*> neighbours;

    for(int dx = -1; dx <= 1; dx++) {
        for(int dy = -1; dy <= 1; dy++) {
            for(int dz = -1; dz <= 1; dz++) {
                glm::ivec3 neighbourCell = this->cell + glm::ivec3(dx, dy, dz);
                uint32_t neighbourHash = this->cellHash(neighbourCell);
                if(neighbourTable.find(neighbourHash) != neighbourTable.end()) {
                    Particle* neighbourParticle = sortedParticles[neighbourTable[neighbourHash]];
                    neighbours.insert(neighbourParticle);
                }
            }
        }
    }

    this->neighbours = std::vector<Particle*>(neighbours.begin(), neighbours.end());
}