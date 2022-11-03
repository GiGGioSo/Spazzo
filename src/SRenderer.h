#ifndef RENDERER_H
#define RENDERER_H

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "object.h"

class SRenderer {

public:
    // Disegno un rettangolo
    static unsigned int generate_vertex_data(); // Create generic VAO

    static void draw_rect(
            const float x, const float y, const float width, const float height,  // global position to draw in and size
            const float rotation,
            unsigned int VAO,
            Shader* shader,
            glm::vec3 color,
            glm::mat4 projection);

    static void draw_object(
            Object* obj,
            glm::mat4 projection);
};

#endif
