#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include "../include/stb_truetype.h"
#include "shader.h"

struct Font {
    const char* filename;
    unsigned int texture;
    stbtt_bakedchar* char_data;
    int first_char;
    int num_chars;
    int font_height;
    int bitmap_width;
    int bitmap_height;
};

struct RendererText {
    unsigned int vao;
    unsigned int vbo;
    unsigned int bytes_offset;
    unsigned int vertex_count;
    Font* fonts;
};

void text_render_init();

void create_texture_font_atlas(Font* font);

void add_text_render_queue(float x, float y, const char* text, Font* font);

void render_text_queue(Font* font, Shader* shader, glm::vec3 color);

#endif
