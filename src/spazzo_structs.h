#ifndef SPAZZO_STRUCTS_H
#define SPAZZO_STRUCTS_H

#include "../include/glm/glm.hpp"
#include "shader.h"

struct Ball {
    unsigned int vertex_data;
    Shader* shader;
    float x, y, w, h;
    float vx, vy;
    float total_vel;
    float rotation;
    float angle_acc;
    float shake_strength;
    glm::vec3 color;
};

struct Platform {
    unsigned int vertex_data;
    Shader* shader;
    float x, y, w, h;
    float vx, vy;
    float max_vel;
    glm::vec3 color;
};

struct GameState {
    unsigned int level;
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

#endif
