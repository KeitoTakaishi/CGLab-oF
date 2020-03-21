#version 150
precision mediump float;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
in vec4 position;
in vec2 texcoord;

out vec2 vTexCoord;

void main(){
    vTexCoord = texcoord;
    //gl_Position = modelViewProjectionMatrix * position;
    vec4 p = position;
    p = proj * view * model * p;
    gl_Position = p;
}