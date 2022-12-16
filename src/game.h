#ifndef GAME_H
#define GAME_H

#include "../include/glm/glm.hpp"
#define GLFW_INCLUDE_NONE
#include "../include/GLFW/glfw3.h"
/* #define MINIAUDIO_IMPLEMENTATION */
#include "miniaudio.h"
#include "shaderer.h"

struct Ball {
    Shader s;
    float x, y, w, h;
    float vx, vy;
    float total_vel;
    float rotation;
    float angle_acc;
    float shake_strength;
    glm::vec3 color;
};

struct Platform {
    Shader s;
    float x, y, w, h;
    float vx, vy;
    float max_vel;
    glm::vec3 color;
};

struct GameState {
    int level;
    unsigned int points;
    bool game_over;
    unsigned int lvl3_ball_touches;
    float last_timestamp; // used to time text on the screen

    // because the shader doesn't work
    glm::mat4 projection;

    // ## DEBUG STUFF ##
    // toggling print info stuff
    bool print_debug_info;
    bool was_debug_info_displayed;
    // fps indicator stuff
    int fps_counter;
    int fps_displayed;
    float fps_time_from_last_update;
};

struct SoundManager {
    ma_engine engine;

    bool bounced;
    ma_sound ball_bounce;

    bool leveled;
    ma_sound level_up;

    ma_sound game_over;
};

void game_init();

void game_free();

void game_reset();

void game_processInput(GLFWwindow* window, float dt);

void game_update(float dt);

void game_render(float dt);


#endif
