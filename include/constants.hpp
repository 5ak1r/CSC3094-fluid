#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cmath>

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;
const unsigned int PARTICLE_ROW_COUNT = 15;
const unsigned int PARTICLE_COUNT = std::pow(PARTICLE_ROW_COUNT, 3.0f);
const float BOX_SIZE = 50.0f;
const float TRANSLATE = BOX_SIZE / PARTICLE_ROW_COUNT;
const float SCALE = 0.01f;
const float GRAVITY = -9.81f;

#endif