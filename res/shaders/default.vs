#version 460 core

layout (location = 0) in vec2 vertex;

uniform mat4 model;
/* uniform mat4 projection;   NON FUNZIONAAAAA  */

void main() {
    gl_Position = model *  vec4(vertex.xy, 0.0, 1.0);
}
