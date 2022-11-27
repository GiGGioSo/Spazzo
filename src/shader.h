#ifndef SHADER_H
#define SHADER_H

#include "../include/glad/glad.h"
#include <string>
#include "../include/glm/glm.hpp"

class Shader {

public:
    // program ID
    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath);

    // activate and use the shader
    void use();

    // utility to interact with uniforms
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, glm::mat4 mat) const;
    void setVec3(const std::string &name, glm::vec3 vec3) const;
};

#endif
