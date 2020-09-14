#version 400

in vec4 position;
in vec2 texcoord;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec2 vTexCoord;

void main(){
    //gl_Position = proj * view * model * position;
    gl_Position = modelViewProjectionMatrix * position;
    vTexCoord = texcoord;
}