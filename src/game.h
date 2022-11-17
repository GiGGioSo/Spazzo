#ifndef GAME_H
#define GAME_H

#include "glm/glm.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class Game {
public:
    unsigned int width, heigth;
    unsigned int level;

public:
    Game(unsigned int width, unsigned int heigth);

    ~Game();

    void init();

    void reset();

    void processInput(GLFWwindow* window, float dt);

    void update(float dt);

    void render(float dt);

};

#endif
