#ifndef TXT_RENDERER_H
#define TXT_RENDERER_H

#include "shader.h"
#include <ft2build.h> 
#include FT_FREETYPE_H
// https://stackoverflow.com/questions/34910660/fatal-error-freetype-config-ftheader-h
// https://stackoverflow.com/questions/12312034/freetype-library-and-undefined-reference-to-ft-init-freetype

#include <glm/glm.hpp>
#include <iostream>
#include <map>

struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

class TextRenderer {
public:

    static std::map<char, Character> default_characters;
    static Shader* default_shader;
    static unsigned int default_VAO;
    static unsigned int default_VBO;

    static int populate_characters_from_font(
            const std::string &font,
            std::map<char, Character> &characters = TextRenderer::default_characters,
            char start = 0,
            char end = 127,
            unsigned int &VAO = TextRenderer::default_VAO,
            unsigned int &VBO = TextRenderer::default_VBO
            );

    static void render_text(
            std::string text,
            float x,
            float y,
            float scale,
            glm::vec3 color,
            std::map<char, Character> characters = TextRenderer::default_characters,
            Shader* s = TextRenderer::default_shader,
            unsigned int VAO = TextRenderer::default_VAO,
            unsigned int VBO = TextRenderer::default_VBO
            );

};

#endif
