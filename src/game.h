#ifndef GAME_H
#define GAME_H

#include "../include/glm/glm.hpp"
#include "spazzo_structs.h"
#define GLFW_INCLUDE_NONE
#include "../include/GLFW/glfw3.h"

class Game {
public:
    unsigned int width, heigth;
    unsigned int level;
    Ball* ball;
    Platform* plat;
    GameState* game_state;

public:
    Game(unsigned int width, unsigned int heigth);

    ~Game();

    void reset();

    void processInput(GLFWwindow* window, float dt);

    void update(float dt);

    void render(float dt);

};

#endif
