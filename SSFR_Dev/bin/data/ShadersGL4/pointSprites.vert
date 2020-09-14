#version 400
uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec2 texcoord;

void main() {
    gl_Position =  position;
}