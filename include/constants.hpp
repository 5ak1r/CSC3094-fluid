#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cmath>

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;
const unsigned int PARTICLE_ROW_COUNT = 10;
const unsigned int PARTICLE_COUNT = std::pow(PARTICLE_ROW_COUNT, 3);
const float SCALE = 0.01f;
const float TRANSLATE = 3.0f;
const float CELL_SIZE = SCALE * TRANSLATE;
const float BOX_SIZE = 50.0f;
const float GRAVITY = -9.81f;

#endif