#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>

/* #include <glm/gtx/string_cast.hpp> */
#include <iostream>
#include <map>

/* #include <ft2build.h>  */ // Already included inside text_renderer.h
/* #include FT_FREETYPE_H */
// https://stackoverflow.com/questions/34910660/fatal-error-freetype-config-ftheader-h
// https://stackoverflow.com/questions/12312034/freetype-library-and-undefined-reference-to-ft-init-freetype

#include "../include/stb_image.h"

#include "shader.h"
#include "SRenderer.h"
#include "game.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void renderText(Shader &s, std::string text, float x, float y, float scale, glm::vec3 color);

float this_frame = 0.f;
float last_frame = 0.f;
float delta_time = 0.f;

const int WIDTH = 800;
const int HEIGTH = 600;
const char* TITLE = "spazzo";

Game game(WIDTH, HEIGTH);

/* struct Character { */
/*     unsigned int TextureID;  // ID handle of the glyph texture */
/*     glm::ivec2   Size;       // Size of glyph */
/*     glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph */
/*     unsigned int Advance;    // Offset to advance to next glyph */
/* }; */
/* std::map<char, Character> characters; */

int main() {

    // GLAD and GLFW setup
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGTH, TITLE, NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // initialize glad
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to inizialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glViewport(0, 0, WIDTH, HEIGTH);

    // FONTS
    //
    // init FreeType2
    /* FT_Library ft; */
    /* if (FT_Init_FreeType(&ft)) */
    /* { */
    /*     std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl; */
    /*     return -1; */
    /* } */
    /* // init font */
    /* FT_Face face; */
    /* if (FT_New_Face(ft, "/usr/share/fonts/Hack Regular Nerd Font Complete.ttf", 0, &face)) { */
    /*     std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl; */
    /*     return -1; */
    /* } */
    /* // Set width and heigth, if width is 0, it gets calculates based on the heigth */
    /* FT_Set_Pixel_Sizes(face, 0, 48); */
    /* // load first 128 glyphs of a font */
    /* glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restrictions */
    /* for (unsigned char c = 0; c < 128; c++) { */
    /*     //load glyph */
    /*     if (FT_Load_Char(face, c, FT_LOAD_RENDER)) { */
    /*         std::cout << "ERROR::FREETYPE: Failed to load Glyph " << c << std::endl; */
    /*         continue; */
    /*     } */
    /*     unsigned int texture; */
    /*     glGenTextures(1, &texture); */
    /*     glBindTexture(GL_TEXTURE_2D, texture); */
    /*     glTexImage2D( */
    /*             GL_TEXTURE_2D, */
    /*             0, */
    /*             GL_RED, */
    /*             face->glyph->bitmap.width, */
    /*             face->glyph->bitmap.rows, */
    /*             0, */
    /*             GL_RED, */
    /*             GL_UNSIGNED_BYTE, */
    /*             face->glyph->bitmap.buffer */
    /*         ); */
    /*     // texture options */
    /*     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); */
    /*     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); */
    /*     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); */
    /*     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); */
    /*     // store the characther inside the map to use it later */
    /*     Character character = { */
    /*         texture, */
    /*         glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows), */
    /*         glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top), */
    /*         (unsigned int) face->glyph->advance.x */
    /*     }; */
    /*     characters.insert(std::pair<char, Character>(c, character)); */
    /* } */
    /* // free resources */
    /* FT_Done_Face(face); */
    /* FT_Done_FreeType(ft); */

    /* // reserving resources for text rendering */
    /* glGenVertexArrays(1, &textVAO); */
    /* glGenBuffers(1, &textVBO); */
    /* glBindVertexArray(textVAO); */
    /* glBindBuffer(GL_ARRAY_BUFFER, textVBO); */
    /* // GL_DYNAMIC_DRAW because we are updating often the data inside of this */
    /* glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW); */
    /* glEnableVertexAttribArray(0); */
    /* glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0); */
    /* glBindBuffer(GL_ARRAY_BUFFER, 0); */
    /* glBindVertexArray(0); */

    // text shader
    /* glm::mat4 text_projection = glm::ortho( */
    /*         0.0f, (float)WIDTH,  */
    /*         0.0f, (float)HEIGTH,  */
    /*         0.0f, 1.0f); */
    /* TextRenderer::default_shader = new Shader("res/shaders/text.vs", "res/shaders/text.fs"); */
    /* TextRenderer::default_shader->use(); */
    /* TextRenderer::default_shader->setMat4("projection", text_projection); */

    /* TextRenderer::populate_characters_from_font( */
    /*         TextRenderer::default_characters, */
    /*         TextRenderer::default_VAO, */
    /*         TextRenderer::default_VBO, */
    /*         "/usr/share/fonts/Hack Regular Nerd Font Complete.ttf", */
    /*         -1, */
    /*         -1); */

    // Start of the actual game
    game.init();

    while (!glfwWindowShouldClose(window)) {

        // Generate delta time
        this_frame = glfwGetTime();
        delta_time = this_frame - last_frame;
        last_frame = this_frame;

        // input
        game.processInput(window, delta_time);

        // update
        game.update(delta_time);

        //draw
        glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        game.render(delta_time);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

/* void renderText(Shader &s, std::string text, float x, float y, float scale, glm::vec3 color) { */
/*     // activate corresponding render state	 */
/*     s.use(); */
/*     s.setVec3("textColor", color); */
/*     glActiveTexture(GL_TEXTURE0); */
/*     glBindVertexArray(textVAO); */

/*     // iterate through all characters */
/*     std::string::const_iterator c; */
/*     for (c = text.begin(); c != text.end(); c++) */
/*     { */
/*         Character ch = characters[*c]; */

/*         float xpos = x + ch.Bearing.x * scale; */
/*         float ypos = y - (ch.Size.y - ch.Bearing.y) * scale; */

/*         float w = ch.Size.x * scale; */
/*         float h = ch.Size.y * scale; */
/*         // update VBO for each character */
/*         float vertices[6][4] = { */
/*             { xpos,     ypos + h,   0.0f, 0.0f },             */
/*             { xpos,     ypos,       0.0f, 1.0f }, */
/*             { xpos + w, ypos,       1.0f, 1.0f }, */

/*             { xpos,     ypos + h,   0.0f, 0.0f }, */
/*             { xpos + w, ypos,       1.0f, 1.0f }, */
/*             { xpos + w, ypos + h,   1.0f, 0.0f }            */
/*         }; */
/*         // render glyph texture over quad */
/*         glBindTexture(GL_TEXTURE_2D, ch.TextureID); */
/*         // update content of VBO memory */
/*         glBindBuffer(GL_ARRAY_BUFFER, textVBO); */
/*         glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);  */
/*         glBindBuffer(GL_ARRAY_BUFFER, 0); */
/*         // render quad */
/*         glDrawArrays(GL_TRIANGLES, 0, 6); */
/*         // now advance cursors for next glyph (note that advance is number of 1/64 pixels) */
/*         x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64) */
/*     } */
/*     glBindVertexArray(0); */
/*     glBindTexture(GL_TEXTURE_2D, 0); */
/* } */

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
