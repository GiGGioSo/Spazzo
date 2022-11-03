#ifndef OBJECT_H
#define OBJECT_H

#include "glm/glm.hpp"
#include "shader.h"

struct Object {
    unsigned int vertex_data;
    Shader* shader;
    float x, y, w, h;
    float vx, vy;
    float rotation;
    glm::vec3 color;
};

#endif
