#include "shader.h"

#include "../include/glad/glad.h"
#include <fstream>
#include "../include/glm/gtc/type_ptr.hpp"
#include <sstream>
#include <iostream>

#include "../include/glm/glm.hpp"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {

    // ----- get shaders from their path -----
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile; // vertex shader file
    std::ifstream fShaderFile; // fragment shader file

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        std::stringstream vShaderStream, fShaderStream;

        // read file contents into the string streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // close files
        vShaderFile.close();
        fShaderFile.close();

        // convert the string stream into a string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

    } catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ:\n" << e.what() << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // ----- compile shaders and create program -----
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // vertex shader
    
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    //print compile errors if any
    glGetShaderiv(fragment, 512, &success);
    if(!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    // print linking errors if any
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // delete the single shaders
    glDeleteShader(vertex);
    glDeleteShader(fragment);

}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setMat4(const std::string &name, glm::mat4 mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setVec3(const std::string &name, glm::vec3 vec3) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), vec3.x, vec3.y, vec3.z);
}
