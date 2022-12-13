#include "quad_renderer.h"
#include "../include/glad/glad.h"

#include "globals.h"

#include <iostream>
#include <math.h>

void quad_render_init() {

    glGenVertexArrays(1, &quad_renderer->vao);
    glGenBuffers(1, &quad_renderer->vbo);

    glBindVertexArray(quad_renderer->vao);
    glBindBuffer(GL_ARRAY_BUFFER, quad_renderer->vbo);

    // NOTE: 50 is the max number of quads displayable together on the screen
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 5 * 50, NULL, GL_DYNAMIC_DRAW);

    // Vertex position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    // Vertex color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (2 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    quad_renderer->bytes_offset = 0;
    quad_renderer->vertex_count = 0;

}

void quad_render_add_queue(float x, float y, float w, float h, float r, glm::vec3 c) {

    float center_x = x + w/2;
    float center_y = y + h/2;

    float vertices[] = {
        x  , y+h, c.x, c.y, c.z,
        x  , y  , c.x, c.y, c.z,
        x+w, y  , c.x, c.y, c.z,
        x  , y+h, c.x, c.y, c.z,
        x+w, y  , c.x, c.y, c.z,
        x+w, y+h, c.x, c.y, c.z,
    };

    r = glm::radians(r);

    for(int i = 0; i < 6; i++) {
        float vx = vertices[i*5 + 0];
        float vy = vertices[i*5 + 1];

        float newX = center_x + (vx - center_x) * cos(r) + (vy - center_y) * sin(r);
        float newY = center_y + (vx - center_x) * sin(r) - (vy - center_y) * cos(r);

        vertices[i*5 + 0] = newX;
        vertices[i*5 + 1] = newY;
    }

    glBindVertexArray(quad_renderer->vao);
    glBindBuffer(GL_ARRAY_BUFFER, quad_renderer->vbo);

    glBufferSubData(GL_ARRAY_BUFFER, quad_renderer->bytes_offset, sizeof(vertices), vertices);

    quad_renderer->bytes_offset += sizeof(vertices);
    quad_renderer->vertex_count += 6;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void quad_render_draw(Shader *shader) {

    shader->use();

    glBindVertexArray(quad_renderer->vao);

    glDrawArrays(GL_TRIANGLES, 0, quad_renderer->vertex_count);

    glBindVertexArray(0);

    quad_renderer->bytes_offset = 0;
    quad_renderer->vertex_count = 0;

}
