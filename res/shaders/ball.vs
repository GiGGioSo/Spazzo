#version 460 core

layout (location = 0) in vec2 vertex;

uniform mat4 model;
/* uniform mat4 projection;   NON FUNZIONAAAAA  */

uniform float time;
uniform bool shake;
uniform float shake_strength;

void main() {
    gl_Position = model *  vec4(vertex.xy, 0.0, 1.0);

    if (shake) {
        gl_Position.x += sin(time * 10) * shake_strength / 3*2;
        gl_Position.y += sin(time * 15) * shake_strength;
    }
}
