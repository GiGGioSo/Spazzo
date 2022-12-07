#ifndef GLOBALS_H
#define GLOBALS_H

#include "text_renderer.h"
#include "input.h"

// all defined in main.cpp
extern const int WIDTH;
extern const int HEIGHT;
extern const char* TITLE;

extern InputController* input;

extern RendererText* text_renderer;

inline void globals_free() {
    delete input;
    delete text_renderer;
}

#endif
