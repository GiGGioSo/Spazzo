#include "../include/glad/glad.h"
/* #include <GLFW/glfw3.h> */
#include "../include/glfw3.h"

#include <iostream>
#include <map>
#include <math.h>
#include <pthread.h>

/* #include <ft2build.h>  */ // Already included inside text_renderer.h
/* #include FT_FREETYPE_H */
// https://stackoverflow.com/questions/34910660/fatal-error-freetype-config-ftheader-h
// https://stackoverflow.com/questions/12312034/freetype-library-and-undefined-reference-to-ft-init-freetype

#include "../include/stb_image.h"

#include "shader.h"
#include "renderer.h"
#include "game.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

float this_frame = 0.f;
float last_frame = 0.f;
float delta_time = 0.f;

const int WIDTH = 800;
const int HEIGTH = 600;
const char* TITLE = "spazzo";

Game* game;

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

    // disable vsync
    glfwSwapInterval(0);

    // initialize glad
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to inizialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glViewport(0, 0, WIDTH, HEIGTH);


    // Start of the actual game
    game = new Game(WIDTH, HEIGTH);

    while (!glfwWindowShouldClose(window)) {

        // Generate delta time
        this_frame = glfwGetTime();
        delta_time = this_frame - last_frame;
        last_frame = this_frame;

        // input
        game->processInput(window, delta_time);

        // update
        game->update(delta_time);

        //draw
        glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        game->render(delta_time);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
