#ifndef INPUT_H
#define INPUT_H

#include "../include/glfw3.h"

struct InputController {
    // Gameplay
    bool move_up;
    bool move_down;
    bool reset;

    // Debug
    bool toggle_debug;
};

/* Updates the InputController global struct
 *
 */
void input_controller_update(GLFWwindow* window);

#endif
