#include "SRenderer.h"
#include "../include/glm/gtx/string_cast.hpp"

unsigned int SRenderer::generate_vertex_data() {

    float vertices[] = {
        0.f, 1.f,       // top left
        1.f, 0.f,       // top right
        0.f, 0.f,       // bottom right
        0.f, 1.f,       // top left
        1.f, 1.f,        // bottom left
        1.f, 0.f       // bottom right
    };
    unsigned int VAO; // vertex array object
    unsigned int VBO; // vertex buffer object
    glGenVertexArrays(1, &VAO); // generate 1 vertex array object
    glGenBuffers(1, &VBO); // generate 1 buffer object at VBO address
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind the VBO to the specific type of buffer (GL_ARRAY_BUFFER)
    // copy data inside the the VERTEX and ELEMENT buffers, using GL_STATIC_DRAW method
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(VAO);
    // tell opengl how to interpret the vertex data, with a vertex attribute pointer
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    // unbind buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // unbind vertex array object
    glBindVertexArray(0);

    // the Vertex Array Object contain all the information to draw the vertices
    return VAO;
}

void SRenderer::draw_rect(
        const float x, const float y, const float width, const float heigth,
        const float rotation,
        unsigned int VAO, 
        Shader *shader, 
        glm::vec3 color,
        glm::mat4 projection) {

    shader->use();
    glm::mat4 model = glm::mat4(1.f);
    /* std::cout << "STEP 1:\n" << glm::to_string(model) << std::endl; */

    model = glm::translate(model, glm::vec3(x, y, 0.f));

    // translate before rotation to make it rotate from the center
    model = glm::translate(model, glm::vec3(0.5f * width, 0.5f * heigth, 0.f));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * width, -0.5f * heigth, 0.f));

    model = glm::scale(model, glm::vec3(width, heigth, 1.0f));

    // I HAVE TO DO THIS HERE BECAUSE DOESN'T WORK INSIDE THE SHADERS
    model = projection * model;

    shader->setMat4("model", model);
    shader->setVec3("spriteColor", color);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SRenderer::draw_object(Object *obj, glm::mat4 projection) {
    obj->shader->use();
    glm::mat4 model = glm::mat4(1.f);
    /* std::cout << "STEP 1:\n" << glm::to_string(model) << std::endl; */

    model = glm::translate(model, glm::vec3(obj->x, obj->y, 0.f));

    // translate before rotation to make it rotate from the center
    model = glm::translate(model, glm::vec3(0.5f * obj->w, 0.5f * obj->h, 0.f));
    model = glm::rotate(model, glm::radians(obj->rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * obj->w, -0.5f * obj->h, 0.f));

    model = glm::scale(model, glm::vec3(obj->w, obj->h, 1.0f));

    // I HAVE TO DO THIS HERE BECAUSE DOESN'T WORK INSIDE THE SHADERS
    model = projection * model;

    obj->shader->setMat4("model", model);
    obj->shader->setVec3("spriteColor", obj->color);

    glBindVertexArray(obj->vertex_data);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
