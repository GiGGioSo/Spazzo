#include "text_renderer.h"
#include "../include/glad/glad.h"

#include "globals.h"

#include "../include/stb_truetype.h"
#include <cstdio>
#include <cstring>
#include <iostream>


void text_render_init() {

    glGenVertexArrays(1, &text_renderer->vao);
    glGenBuffers(1, &text_renderer->vbo);

    glBindVertexArray(text_renderer->vao);
    glBindBuffer(GL_ARRAY_BUFFER, text_renderer->vbo);

    // 1000 is the max number of character displayable together on the screen
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4 * 1000, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    text_renderer->vertex_count = 0;
    text_renderer->bytes_offset = 0;
    text_renderer->fonts = (Font*) malloc(sizeof(Font) * 1);

    Font* f = &text_renderer->fonts[0];
    f->filename = "/usr/share/fonts/TTF/DejaVuSans.ttf";
    f->first_char = 32;
    f->num_chars = 96;
    f->font_height = 32.0f;
    f->bitmap_width = 512;
    f->bitmap_height = 512;
    f->char_data = (stbtt_bakedchar*) malloc(sizeof(stbtt_bakedchar) * f->num_chars);
    create_texture_font_atlas(f);


}

void create_texture_font_atlas(Font* font) {

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restrictions

    unsigned char ttf_buffer[1<<20];
    unsigned char temp_bitmap[font->bitmap_width * font->bitmap_height];

    fread(ttf_buffer, 1, 1<<20, fopen(font->filename, "rb"));

    stbtt_BakeFontBitmap(ttf_buffer, 0, font->font_height, temp_bitmap, font->bitmap_width, font->bitmap_height, font->first_char, font->num_chars, font->char_data);

    glGenTextures(1, &font->texture);
    glBindTexture(GL_TEXTURE_2D, font->texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, font->bitmap_width, font->bitmap_height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, temp_bitmap);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}



void add_text_render_queue(float x, float y, const char *text, Font* font) {
    int length = strlen(text);

    float vertices[length * 6][4];

    for(int i = 0; i < length; i++) {
        if (text[i] >= font->first_char && text[i] < font->first_char+font->num_chars) {

            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(font->char_data, font->bitmap_width, font->bitmap_height, text[i] - font->first_char, &x, &y, &q, 1);

            // down left
            vertices[i*6 + 0][0] = q.x0;
            vertices[i*6 + 0][1] = q.y1;
            vertices[i*6 + 0][2] = q.s0;
            vertices[i*6 + 0][3] = q.t1;
            // top left
            vertices[i*6 + 1][0] = q.x0;
            vertices[i*6 + 1][1] = q.y0;
            vertices[i*6 + 1][2] = q.s0;
            vertices[i*6 + 1][3] = q.t0;
            // top right
            vertices[i*6 + 2][0] = q.x1;
            vertices[i*6 + 2][1] = q.y0;
            vertices[i*6 + 2][2] = q.s1;
            vertices[i*6 + 2][3] = q.t0;
            // down left
            vertices[i*6 + 3][0] = q.x0;
            vertices[i*6 + 3][1] = q.y1;
            vertices[i*6 + 3][2] = q.s0;
            vertices[i*6 + 3][3] = q.t1;
            // top right
            vertices[i*6 + 4][0] = q.x1;
            vertices[i*6 + 4][1] = q.y0;
            vertices[i*6 + 4][2] = q.s1;
            vertices[i*6 + 4][3] = q.t0;
            // down left
            vertices[i*6 + 5][0] = q.x1;
            vertices[i*6 + 5][1] = q.y1;
            vertices[i*6 + 5][2] = q.s1;
            vertices[i*6 + 5][3] = q.t1;
        }
    }

    glBindVertexArray(text_renderer->vao);
    glBindBuffer(GL_ARRAY_BUFFER, text_renderer->vbo);

    glBufferSubData(GL_ARRAY_BUFFER, text_renderer->bytes_offset, sizeof(vertices), vertices);

    text_renderer->bytes_offset += sizeof(vertices);

    text_renderer->vertex_count += length * 6;
    


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void render_text_queue(Font* font, Shader *shader, glm::vec3 color) {

    shader->use();
    shader->setVec3("textColor", color);

    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(text_renderer->vao);
    glBindTexture(GL_TEXTURE_2D, font->texture);

    glDrawArrays(GL_TRIANGLES, 0, text_renderer->vertex_count);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    text_renderer->bytes_offset = 0;
    text_renderer->vertex_count = 0;
}

