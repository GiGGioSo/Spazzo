#include "game.h"
#include "shader.h"
#include "renderer.h"
#include "txt_renderer.h"
#include "spazzo_structs.h"
#include "../include/glfw3.h"
#include <cmath>
#include <cstdlib>
#include "../include/glm/common.hpp"
#include "../include/glm/ext.hpp"
#include <iostream>
#include <string>
#include "math.h"

// Custom font for the change of level indication
std::map<char, Character> level_font_characters;
Shader* level_text_shader;

Game::Game(unsigned int width, unsigned int heigth) {
    this->width = width;
    this->heigth = heigth;
    this->level = -1;

    // Text rendering initialization
    glm::mat4 text_projection = glm::ortho(
            0.0f, (float)width, 
            0.0f, (float)heigth, 
            0.0f, 1.0f);
    // text shader
    TextRenderer::default_shader = new Shader("res/shaders/text.vs", "res/shaders/text.fs");
    TextRenderer::default_shader->use();
    TextRenderer::default_shader->setMat4("projection", text_projection);

    level_text_shader = new Shader("res/shaders/level_font.vs", "res/shaders/text.fs");
    level_text_shader->use();
    level_text_shader->setMat4("projection", text_projection);
    
    // creating all the fonts
    TextRenderer::populate_characters_from_font(
            "/usr/share/fonts/TTF/DejaVuSans.ttf");
    TextRenderer::populate_characters_from_font(
            "/usr/share/fonts/Hack Bold Nerd Font Complete.ttf",
            //"/usr/share/fonts/Hack Bold Nerd Font Complete Mono.ttf",
            level_font_characters);


    srand((unsigned)time(0));


    game_state = new GameState;
    game_state->game_over = false;
    game_state->points = 0;
    game_state->lvl3_ball_touches = 0;
    game_state->print_debug_info = true;
    game_state->projection = glm::ortho(0.0f, (float)width, (float)heigth, 0.0f, -1.0f, 1.0f);
    game_state->was_debug_info_displayed = false;
    game_state->fps_counter = 0;
    game_state->fps_displayed = 0;
    game_state->fps_time_from_last_update = 0.0f;

    ball = new Ball;
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

    plat = new Platform;
    plat->max_vel = 250.f;
    plat->vertex_data = Renderer::generate_vertex_data();
    plat->shader = new Shader("res/shaders/default.vs", "res/shaders/default.fs");
    plat->w= 20.f;
    plat->h= 200.f;
    plat->y = heigth / 2.f - plat->h / 2.f;
    plat->x = 0.0f;
    plat->vx = 0.f;
    plat->vy = 0.f;
    plat->color = glm::vec3(0.9f, 0.5f, 0.3f);
}

Game::~Game() {
    delete level_text_shader;
    delete ball;
    delete plat;
    delete game_state;
}

void Game::processInput(GLFWwindow* window, float dt) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    // Fetching controller input
    // ANALOGICO
    float lj_h; // left-joysticj horizontal  -1=left   1=right
    float lj_v; // left-joystick vertical    -1=up     1=down
    float rj_h; // right-joysticj horizontal  -1=left   1=right
    float rj_v; // right-joystick vertical    -1=up     1=down
    float lt_an; // analogical left-trigger   -1=released     1=pressed
    float rt_an; // analogical right-trigger   -1=released     1=pressed

    //DIGITALE
    bool dpad_up; // DPAD controls
    bool dpad_right;
    bool dpad_down;
    bool dpad_left;

    bool b_up; // Button controls (I put the direction instead of the name)
    bool b_right;
    bool b_down;
    bool b_left;

    bool ls; // left shoulder
    bool rs; // right shoulder

    bool lt_dig; // digital left trigger
    bool rt_dig; // digital right trigger

    bool lj_click; // right-joystick click
    bool rj_click; // left-joystick click

    bool b_share; // share button
    bool b_opt; // option button
    bool b_logo; // logo button

    int c1_present = glfwJoystickPresent(GLFW_JOYSTICK_1);
    if (!c1_present); //std::cout << "[ERROR] Controller 1 not present" << std::endl;
    else {
        int axes_count;
        const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axes_count);
        if (axes_count != 6) std::cout << "[WARNING] Controller with " << axes_count << " axes!!" << std::endl;
        else {
            lj_h = axes[0];
            lj_v = axes[1];
            lt_an = axes[2];
            rj_h = axes[3];
            rj_v = axes[4];
            rt_an = axes[5];
        }
        int buttons_count;
        const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttons_count);
        if (buttons_count != 17) std::cout << "[WARNING] Controller with " << buttons_count << " buttons!!" << std::endl;
        else {
            b_down = buttons[0] == GLFW_PRESS;
            b_right = buttons[1] == GLFW_PRESS;
            b_up = buttons[2] == GLFW_PRESS;
            b_left = buttons[3] == GLFW_PRESS;
            ls = buttons[4] == GLFW_PRESS;
            rs = buttons[5] == GLFW_PRESS;
            lt_dig = buttons[6] == GLFW_PRESS;
            rt_dig = buttons[7] == GLFW_PRESS;
            b_share = buttons[8] == GLFW_PRESS;
            b_opt = buttons[9] == GLFW_PRESS;
            b_logo = buttons[10] == GLFW_PRESS;
            lj_click = buttons[11] == GLFW_PRESS;
            rj_click = buttons[12] == GLFW_PRESS;
            dpad_up = buttons[13] == GLFW_PRESS;
            dpad_right = buttons[14] == GLFW_PRESS;
            dpad_down = buttons[15] == GLFW_PRESS;
            dpad_left = buttons[16] == GLFW_PRESS;
        }
    }

    // Platform movement
    plat->vy = 0;
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ||
            (c1_present && (lj_v <= -0.7f || dpad_up))) {

        plat->vy = -plat->max_vel;

    } else if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ||
            (c1_present && (lj_v >= 0.7f || dpad_down))) {

        plat->vy = plat->max_vel;

    }

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        if (!game_state->was_debug_info_displayed)
            game_state->print_debug_info = !game_state->print_debug_info;

        game_state->was_debug_info_displayed = true;
    } else {
        game_state->was_debug_info_displayed = false;
    }

    // Reset game
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS ||
        (c1_present && b_up)) {
        if (ball->vx == 0.f && ball->vy == 0.f && ball->x == 0.f) {
            this->reset();
        }
    }
}

void Game::update(float dt) {

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

    if (game_state->game_over) return;


    // Ball movement
    ball->x += ball->vx * dt;
    ball->y += ball->vy * dt;
    if (ball->x + ball->w > width){ //right bound
        ball->x = width - ball->w;
        ball->vx *= -1;
    }
    if (ball->y < 0) { // upper bound
        ball->y = 0.f;
        ball->rotation *= -1; // don't directly touch vy of the ball, just change the angle
    }
    if (ball->y + ball->h > heigth) { // downer bound
        ball->y = heigth - ball->h;
        ball->rotation *= -1;
    }

    // Platform movement
    plat->y += plat->vy * dt;
    if (plat->y <= 0) plat->y = 0;
    if (plat->y + plat->h >= heigth) plat->y = heigth - plat->h;

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
        switch (level) {
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
    if (level == 1 && ball->total_vel >= 800) { // 1000
        level = 2;
        game_state->last_timestamp = frame_time;
        ball->color = glm::vec3(0.8f, 0.8f, 0.2f);
        std::cout << "LEVEL 2!" << std::endl;
    } else if (level == 2 && plat->h <= 130) { // 120
        level = 3;
        game_state->last_timestamp = frame_time;
        ball->color = glm::vec3(0.8f, 0.2f, 0.2f);
        std::cout << "LEVEL 3!" << std::endl;
    } else if (level == 3 && game_state->lvl3_ball_touches > 5) { // 3
        level = 4;
        game_state->last_timestamp = frame_time;
        ball->color = glm::vec3(0.1f, 0.0f, 0.0f);
        ball->shader->use();
        ball->shader->setBool("shake", true);
        ball->shader->setFloat("shake_strength", ball->shake_strength);
        std::cout << "LEVEL 4!" << std::endl;
    } else if (level == 4 && ball->shake_strength > 0.020) {
        level = 5;
        game_state->last_timestamp = frame_time;
        std::cout << "FINAL LEVEL!" << std::endl;
    }

    if (level >= 4) {
        ball->shader->use();
        ball->shader->setFloat("time", frame_time);
    }


    // Game over condition
    if (ball->x <= 0 && ball->vx != 0.f) { // left bound and game is not over
        game_state->game_over = true;
        this->level = -1;
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

void Game::render(float dt) {

    float frame_time = (float) glfwGetTime();

    Renderer::draw_rect(plat->x, plat->y, plat->w, plat->h, 0.0f, plat->vertex_data, plat->shader, plat->color, game_state->projection);
    Renderer::draw_rect(ball->x, ball->y, ball->w, ball->h, -glm::sign(ball->vx)*ball->rotation, ball->vertex_data, ball->shader, ball->color, game_state->projection);


    // Points
    TextRenderer::render_text(
            "Points: "+std::to_string(game_state->points),
            330.0f,
            (float)heigth - 40.0f,
            0.65f,
            glm::vec3(0.1f, 0.1f, 0.1f));

    // Level advancement render
    if (this->level == 1 && (frame_time - game_state->last_timestamp) < 2)
        TextRenderer::render_text(
                "LEVEL 1!",
                300.0f,
                (float)heigth / 2.f,
                1.0f,
                glm::vec3(0.1f, 0.1f, 0.1f),
                level_font_characters,
                level_text_shader);
    if (this->level == 2 && (frame_time - game_state->last_timestamp) < 2)
        TextRenderer::render_text(
                "LEVEL 2!",
                300.0f,
                (float)heigth / 2.f,
                1.0f,
                glm::vec3(0.1f, 0.1f, 0.1f),
                level_font_characters,
                level_text_shader);
    if (this->level == 3 && (frame_time - game_state->last_timestamp) < 2)
        TextRenderer::render_text(
                "LEVEL 3!",
                300.0f,
                (float)heigth / 2.f,
                1.0f,
                glm::vec3(0.1f, 0.1f, 0.1f),
                level_font_characters,
                level_text_shader);
    if (this->level == 4 && (frame_time - game_state->last_timestamp) < 2)
        TextRenderer::render_text(
                "LEVEL 4!",
                300.0f,
                (float)heigth / 2.f,
                1.0f,
                glm::vec3(0.1f, 0.1f, 0.1f),
                level_font_characters,
                level_text_shader);
    if (this->level == 5 && (frame_time - game_state->last_timestamp) < 2)
        TextRenderer::render_text(
                "FINAL LEVEL!",
                300.0f,
                (float)heigth / 2.f,
                1.0f,
                glm::vec3(0.1f, 0.1f, 0.1f),
                level_font_characters,
                level_text_shader);
    if (game_state->game_over) { // FIXME: La shader non viene applicata
        TextRenderer::render_text(
                "GAME OVER",
                300.0f,
                (float)heigth / 2.f,
                1.0f,
                glm::vec3(0.1f, 0.1f, 0.1f),
                level_font_characters,
                level_text_shader);
        TextRenderer::render_text(
                "You scored "+std::to_string(game_state->points)+" points!",
                290.0f,
                (float)heigth / 2.f - 40.0f,
                0.5f,
                glm::vec3(0.1f, 0.1f, 0.1f),
                level_font_characters);
    }
    
    if (game_state->print_debug_info) {
        // Movement info
        TextRenderer::render_text(
                "Angle: "+std::to_string(ball->rotation),
                10.0f,
                10.0f,
                0.4f,
                glm::vec3(0.1f, 0.1f, 0.1f));
        TextRenderer::render_text(
                "Velocity: "+std::to_string(ball->total_vel),
                10.0f,
                30.0f,
                0.4f,
                glm::vec3(0.1f, 0.1f, 0.1f));

        // FPS counter
        TextRenderer::render_text(
                "FPS: "+std::to_string(game_state->fps_displayed),
                (float) width/2 - 20,
                10.0f,
                0.4f,
                glm::vec3(0.1f, 0.1f, 0.1f));
    }
}

void Game::reset() {
    std::cout << "\n------------------------------\n-------- Game started! -------\n------------------------------\n" << std::endl;
    game_state->game_over = false;
    ball->total_vel = 500.f;
    ball->rotation = rand() % 120 - 60;
    ball->angle_acc = 0.f;
    ball->x = width / 5.f;
    ball->y = heigth / 2.f;
    ball->vx = cos(glm::radians(ball->rotation)) * ball->total_vel;
    ball->vy = sin(glm::radians(ball->rotation)) * ball->total_vel * -1;
    ball->color = glm::vec3(0.2f, 0.8f, 0.2f);

    ball->shake_strength = 0.005;
    game_state->lvl3_ball_touches = 0;
    
    game_state->points = 0;
    plat->max_vel = 250.f;
    plat->y -= (200.f - plat->h) / 2.f;
    if (plat->y + 200 > heigth)
        plat->y = heigth - 200;
    plat->h = 200.f;

    level = 1;
    std::cout << "LEVEL 1!" << std::endl;
    game_state->last_timestamp = glfwGetTime();
}
