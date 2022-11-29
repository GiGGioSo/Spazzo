#ifndef GAME_H
#define GAME_H

#include "../include/glm/glm.hpp"
#include "spazzo_structs.h"
#define GLFW_INCLUDE_NONE
#include "../include/GLFW/glfw3.h"

void game_init();

void game_free();

void game_reset();

void game_processInput(GLFWwindow* window, float dt);

void game_update(float dt);

void game_render(float dt);


#endif
