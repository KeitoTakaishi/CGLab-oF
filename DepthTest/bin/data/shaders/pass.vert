#version 150
uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec2  texcoord;

out vec2 Texcoord;
void main(){
    Texcoord = texcoord;
    //gl_Position = modelViewProjectionMatrix * position;
    gl_Position = position;
}