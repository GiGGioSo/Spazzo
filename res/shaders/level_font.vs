#version 460 core

layout (location = 0) in vec4 vertex;
out vec2 TexCoords;

uniform mat4 projection;
uniform float time;

void main() {
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;

    float strength = 0.05;
    gl_Position.y += sin(time*5 + gl_Position.x * 5.0) * strength;
}
