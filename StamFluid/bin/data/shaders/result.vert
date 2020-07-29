#version 150
precision mediump float;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform vec2 screen;

in vec4 position;
in vec2 texcoord;

out vec2 vTexCoord;
out vec2 vVelCoord;
void main() {
    vec2 tc = texcoord;
    vVelCoord = tc;
    tc.y = 512 - tc.y;
    vTexCoord = tc;

    vec4 p = position;
    //p = modelViewProjectionMatrix * p;
    //p = proj * view * model * p;
    gl_Position = p;
}