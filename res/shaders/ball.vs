#version 460 core

layout (location = 0) in vec2 inPos;
layout (location = 1) in vec3 inColor;

out vec3 vColor;

uniform mat4 projection;

uniform float time;
uniform bool shake;
uniform float shake_strength;

void main() {
    gl_Position = projection *  vec4(inPos.xy, 0.0, 1.0);

    if (shake) {
        gl_Position.x += sin(time * 10) * shake_strength / 3*2;
        gl_Position.y += sin(time * 15) * shake_strength;
    }

    vColor = inColor;
}
