#include "../include/glad/glad.h"
/* #include <GLFW/glfw3.h> */
#include "../include/glfw3.h"

#include <cstdio>
#include <iostream>
#include <map>
#include <math.h>
#include <pthread.h>

/* #include <ft2build.h>  */ // Already included inside text_renderer.h
/* #include FT_FREETYPE_H */
// https://stackoverflow.com/questions/34910660/fatal-error-freetype-config-ftheader-h
// https://stackoverflow.com/questions/12312034/freetype-library-and-undefined-reference-to-ft-init-freetype

#include "globals.h"

#include "../include/glm/common.hpp"
#include "../include/glm/ext.hpp"

/* #include "../include/stb_image.h" */
#include "quad_renderer.h"
#include "text_renderer.h"
#define STB_TRUETYPE_IMPLEMENTATION  // force following include to generate implementation
#include "../include/stb_truetype.h"

#define MINIAUDIO_IMPLEMENTATION
#include "../include/miniaudio.h"

#include "shader.h"
#include "renderer.h"
#include "game.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void debug_callback(
    GLenum source, GLenum type, GLuint id, GLenum severity,
    GLsizei length, const GLchar* message, const void* user);

float this_frame = 0.f;
float last_frame = 0.f;
float delta_time = 0.f;

// GLOBALS
const int WIDTH = 800;
const int HEIGHT = 600;
const char* TITLE = "spazzo";
InputController* input = new InputController();
RendererText* text_renderer = new RendererText();
RendererQuad* quad_renderer = new RendererQuad();

int main() {

    // GLAD and GLFW setup
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // disable vsync
    glfwSwapInterval(0);

    // initialize glad
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to inizialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // enable debug
    glDebugMessageCallback(&debug_callback, NULL);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glViewport(0, 0, WIDTH, HEIGHT);

    text_render_init();
    quad_render_init();

    game_init();

    while (!glfwWindowShouldClose(window)) {

        // Generate delta time
        this_frame = glfwGetTime();
        delta_time = this_frame - last_frame;
        last_frame = this_frame;

        // input
        update_InputController(window);

        // update
        game_update(delta_time);

        //draw
        glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        game_render(delta_time);


        // Swap buffers and get events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    game_free();
    globals_free();

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

static void debug_callback(
    GLenum source, GLenum type, GLuint id, GLenum severity,
    GLsizei length, const GLchar* message, const void* user)
{
    std::cout << message << std::endl;
    /* if (severity == GL_DEBUG_SEVERITY_HIGH || severity == GL_DEBUG_SEVERITY_MEDIUM) */
    /* { */
    /*     std::cout << "OpenGL API usage error! Use debugger to examine call stack!" << std::endl; */
    /* } */
}
