#include "game.h"
#include "shader.h"
#include "renderer.h"
#include "text_renderer.h"
#include "spazzo_structs.h"
#include "../include/glfw3.h"
#include <cmath>
#include <cstdlib>
#include "../include/glm/common.hpp"
#include "../include/glm/ext.hpp"
#include <iostream>
#include <string>
#include "math.h"

#include "globals.h"

// Custom font for the change of level indication
Shader* level_text_shader;
Shader* default_text_shader;

Ball* ball;
Platform* plat;
GameState* game_state;


void game_init() {

    // Text rendering initialization
    glm::mat4 text_projection = glm::ortho(
            0.0f, (float)WIDTH, 
            0.0f, (float)HEIGHT, 
            0.0f, 1.0f);
    // text shader
    default_text_shader = new Shader("res/shaders/default_text.vs", "res/shaders/default_text.fs");
    default_text_shader->use();
    default_text_shader->setMat4("projection", text_projection);

    level_text_shader = new Shader("res/shaders/level_font.vs", "res/shaders/default_text.fs");
    level_text_shader->use();
    level_text_shader->setMat4("projection", text_projection);

    srand((unsigned)time(0));

    game_state = new GameState();
    game_state->level = -1;
    game_state->game_over = true;
    game_state->points = 0;
    game_state->lvl3_ball_touches = 0;
    game_state->print_debug_info = true;
    game_state->projection = glm::ortho(0.0f, (float)WIDTH, (float)HEIGHT, 0.0f, -1.0f, 1.0f);
    game_state->was_debug_info_displayed = false;
    game_state->fps_counter = 0;
    game_state->fps_displayed = 0;
    game_state->fps_time_from_last_update = 0.0f;

    ball = new Ball();
    ball->shake_strength = 0.005; // default shake strength, used in the shader
    ball->total_vel = 0.f;
    ball->angle_acc = 0.f;
    ball->rotation = 20.f;
    ball->vertex_data = Renderer::generate_vertex_data();
    ball->shader = new Shader("res/shaders/ball.vs", "res/shaders/default.fs");
    ball->x = 0.f;
    ball->y = 50.f;
    ball->w= 30.f;
    ball->h= 30.f;
    ball->vx = cos(glm::radians(ball->rotation)) * ball->total_vel;
    ball->vy = sin(glm::radians(ball->rotation)) * ball->total_vel;
    ball->color = glm::vec3(0.2f, 0.8f, 0.2f);

    plat = new Platform();
    plat->max_vel = 250.f;
    plat->vertex_data = Renderer::generate_vertex_data();
    plat->shader = new Shader("res/shaders/default.vs", "res/shaders/default.fs");
    plat->w= 20.f;
    plat->h= 200.f;
    plat->y = HEIGHT / 2.f - plat->h / 2.f;
    plat->x = 0.0f;
    plat->vx = 0.f;
    plat->vy = 0.f;
    plat->color = glm::vec3(0.9f, 0.5f, 0.3f);
}

void game_free() {
    delete level_text_shader;
    delete ball;
    delete plat;
    delete game_state;
}

void game_update(float dt) {

    // FPS counter stuff
    game_state->fps_time_from_last_update += dt;
    game_state->fps_counter++;
    if (game_state->fps_time_from_last_update > 1) {
        game_state->fps_time_from_last_update -= 1;
        game_state->fps_displayed = game_state->fps_counter;
        game_state->fps_counter = 0;
    }

    // I use it ofter, might aswell calculate it just one time
    float frame_time = (float) glfwGetTime();

    // update time uniform even if game_state->game_over
    level_text_shader->use();
    level_text_shader->setFloat("time", frame_time);

    // Reset game

    if(input->reset && game_state->game_over) {
        game_reset();
    }

    if (game_state->game_over) return;

    // update stuff based on input
    // Platform movement
    plat->vy = 0;
    if(input->move_up) {

        plat->vy = -plat->max_vel;

    } else if(input->move_down) {

        plat->vy = plat->max_vel;

    }
    // debug info
    if (input->toggle_debug) {
        if (!game_state->was_debug_info_displayed) {
            game_state->print_debug_info = !game_state->print_debug_info;
        }

        game_state->was_debug_info_displayed = true;
    } else {
        game_state->was_debug_info_displayed = false;
    }

    // Ball movement
    ball->x += ball->vx * dt;
    ball->y += ball->vy * dt;
    if (ball->x + ball->w > WIDTH){ //right bound
        ball->x = WIDTH - ball->w;
        ball->vx *= -1;
    }
    if (ball->y < 0) { // upper bound
        ball->y = 0.f;
        ball->rotation *= -1; // don't directly touch vy of the ball, just change the angle
    }
    if (ball->y + ball->h > HEIGHT) { // downer bound
        ball->y = HEIGHT - ball->h;
        ball->rotation *= -1;
    }

    // Platform movement
    plat->y += plat->vy * dt;
    if (plat->y <= 0) plat->y = 0;
    if (plat->y + plat->h >= HEIGHT) plat->y = HEIGHT - plat->h;

    // Collision between ball and platform
    if (ball->x < plat->x+plat->w && // ball is on the same vertical level of the platform
        ball->vx < 0 &&
        !(ball->y > plat->y+plat->h || ball->y+ball->h < plat->y)) { // is NOT outside of the platform

        //increase points
        game_state->points += 1;

        // reset ball position at the platform coordinates
        ball->x = plat->x+plat->w;

        // calculate ball angle based on where it hit the platform
        float angle_multiplier = (plat->y+plat->h - ball->y) / (plat->h + ball->h);
        ball->rotation = 160.f * angle_multiplier - 80.f; // symmetric interval around 0
        ball->vx *= -1;

        // Increase difficolty in different ways, one after another, based on the level
        switch (game_state->level) {
            case 1:
                ball->total_vel += 50.f;
                plat->max_vel += 15.f;
                break;
            case 2:
                plat->y += 5;
                plat->h -= 10;
                break;
            case 3:
                game_state->lvl3_ball_touches += 1;
                plat->max_vel += 10.f;
                ball->vy = sin(glm::radians(ball->rotation)) * ball->total_vel * -1; // * -1 because y is inverted
                ball->angle_acc = ball->vy / 10.f;
                break;
            case 4:
                ball->shake_strength += 0.002;
                ball->shader->use();
                ball->shader->setFloat("shake_strength", ball->shake_strength);
                // I still want to apply effect on the ball
                ball->vy = sin(glm::radians(ball->rotation)) * ball->total_vel * -1; // * -1 because y is inverted
                ball->angle_acc = ball->vy / 10.f;
                break;
            case 5:
                ball->total_vel += 30.f;
                plat->max_vel += 10.f;
                break;
        }
    }

    // update velocities and angle only if its moving
    if (ball->total_vel) {
        // update ball angle based on the accelleration, but stop after a certain angle
        if (glm::abs(ball->rotation) < 50)
            ball->rotation += ball->angle_acc * dt;

        // update ball velocity base on the angle
        ball->vx = cos(glm::radians(ball->rotation)) * ball->total_vel * glm::sign(ball->vx);
        ball->vy = sin(glm::radians(ball->rotation)) * ball->total_vel * -1; // * -1 because y is inverted
    }

    // change level when needed
    if (game_state->level == 1 && ball->total_vel >= 800) { // 1000
        game_state->level = 2;
        game_state->last_timestamp = frame_time;
        ball->color = glm::vec3(0.8f, 0.8f, 0.2f);
        std::cout << "LEVEL 2!" << std::endl;
    } else if (game_state->level == 2 && plat->h <= 130) { // 120
        game_state->level = 3;
        game_state->last_timestamp = frame_time;
        ball->color = glm::vec3(0.8f, 0.2f, 0.2f);
        std::cout << "LEVEL 3!" << std::endl;
    } else if (game_state->level == 3 && game_state->lvl3_ball_touches > 5) { // 3
        game_state->level = 4;
        game_state->last_timestamp = frame_time;
        ball->color = glm::vec3(0.1f, 0.0f, 0.0f);
        ball->shader->use();
        ball->shader->setBool("shake", true);
        ball->shader->setFloat("shake_strength", ball->shake_strength);
        std::cout << "LEVEL 4!" << std::endl;
    } else if (game_state->level == 4 && ball->shake_strength > 0.020) {
        game_state->level = 5;
        game_state->last_timestamp = frame_time;
        std::cout << "FINAL LEVEL!" << std::endl;
    }

    if (game_state->level >= 4) {
        ball->shader->use();
        ball->shader->setFloat("time", frame_time);
    }


    // Game over condition
    if (ball->x <= 0 && ball->vx != 0.f) { // left bound and game is not over
        game_state->game_over = true;
        game_state->level = -1;
        std::cout << "\nGame Over!" << std::endl;
        game_state->last_timestamp = frame_time;
        ball->x = 0.f;
        ball->vx = 0.f;
        ball->vy = 0.f;
        ball->total_vel = 0.f;
        ball->shader->use();
        ball->shader->setBool("shake", false);
        ball->color = glm::vec3(0.8f, 0.8f, 0.8f);
    }
}

void game_render(float dt) {

    float frame_time = (float) glfwGetTime();

    Renderer::draw_rect(plat->x, plat->y, plat->w, plat->h, 0.0f, plat->vertex_data, plat->shader, plat->color, game_state->projection);
    Renderer::draw_rect(ball->x, ball->y, ball->w, ball->h, -glm::sign(ball->vx)*ball->rotation, ball->vertex_data, ball->shader, ball->color, game_state->projection);

    Font* f = &text_renderer->fonts[0];

    // Level advancement render
    if (game_state->level == 1 && (frame_time - game_state->last_timestamp) < 2)
        add_text_render_queue(
                300.0f,
                (float)HEIGHT / 2.f,
                "LEVEL 1!",
                f);
    if (game_state->level == 2 && (frame_time - game_state->last_timestamp) < 2)
        add_text_render_queue(
                300.0f,
                (float)HEIGHT / 2.f,
                "LEVEL 2!",
                f);
    if (game_state->level == 3 && (frame_time - game_state->last_timestamp) < 2)
        add_text_render_queue(
                300.0f,
                (float)HEIGHT / 2.f,
                "LEVEL 3!",
                f);
    if (game_state->level == 4 && (frame_time - game_state->last_timestamp) < 2)
        add_text_render_queue(
                300.0f,
                (float)HEIGHT / 2.f,
                "LEVEL 4!",
                f);
    if (game_state->level == 5 && (frame_time - game_state->last_timestamp) < 2)
        add_text_render_queue(
                300.0f,
                (float)HEIGHT / 2.f,
                "LEVEL 5!",
                f);
    if (game_state->game_over && game_state->level != -1) { // FIXME: La shader non viene applicata
        add_text_render_queue(
                300.0f,
                (float)HEIGHT / 2.f,
                "GAME OVER!",
                f);
        add_text_render_queue(
                300.0f,
                (float)HEIGHT / 2.f,
                ("You scored "+std::to_string(game_state->points)+" points!").c_str(),
                f);
    }

    render_text_queue(f, level_text_shader, glm::vec3(0.0f, 0.0f, 0.0f));

    // Points
    add_text_render_queue(
            330.0f,
            (float)HEIGHT - 40.0f,
            ("Points: "+std::to_string(game_state->points)).c_str(),
            f);

    if (game_state->print_debug_info) {
        // Movement info
        add_text_render_queue(
                30.0f,
                25.0f,
                ("Angle: "+std::to_string(ball->rotation)).c_str(),
                f);
        add_text_render_queue(
                20.0f,
                50.0f,
                ("Velocity: "+std::to_string(ball->total_vel)).c_str(),
                f);

        // FPS counter
        add_text_render_queue(
                (float) WIDTH/2 - 20,
                10.0f,
                ("FPS: "+std::to_string(game_state->fps_displayed)).c_str(),
                f);
    }

    render_text_queue(f, default_text_shader, glm::vec3(0.0f, 0.0f, 0.0f));

}

void game_reset() {
    std::cout << "\n------------------------------\n-------- Game started! -------\n------------------------------\n" << std::endl;
    game_state->game_over = false;
    ball->total_vel = 500.f;
    ball->rotation = rand() % 120 - 60;
    ball->angle_acc = 0.f;
    ball->x = WIDTH / 5.f;
    ball->y = HEIGHT / 2.f;
    ball->vx = cos(glm::radians(ball->rotation)) * ball->total_vel;
    ball->vy = sin(glm::radians(ball->rotation)) * ball->total_vel * -1;
    ball->color = glm::vec3(0.2f, 0.8f, 0.2f);

    ball->shake_strength = 0.005;
    game_state->lvl3_ball_touches = 0;
    
    game_state->points = 0;
    plat->max_vel = 250.f;
    plat->y -= (200.f - plat->h) / 2.f;
    if (plat->y + 200 > HEIGHT)
        plat->y = HEIGHT - 200;
    plat->h = 200.f;

    game_state->level = 1;
    std::cout << "LEVEL 1!" << std::endl;
    game_state->last_timestamp = glfwGetTime();
}
