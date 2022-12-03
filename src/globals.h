#ifndef GLOBALS_H
#define GLOBALS_H

#include "input.h"

// all defined in main.cpp
extern const int WIDTH;
extern const int HEIGHT;
extern const char* TITLE;

extern InputController* input;

inline void globals_free() {
    delete input;
}

#endif
