#ifndef QUAD_RENDERER_H
#define QUAD_RENDERER_H

#include "../include/glm/glm.hpp"
#include "shader.h"

struct RendererQuad {
    unsigned int vao;
    unsigned int vbo;
    unsigned int bytes_offset;
    unsigned int vertex_count;
};

void quad_render_init();

void quad_render_add_queue(float x, float y, float w, float h, float r, glm::vec3 c);

void quad_render_draw(Shader* shader);

#endif
