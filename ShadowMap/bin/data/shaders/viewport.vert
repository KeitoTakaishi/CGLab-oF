#version 150
uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec2  texcoord;

out vec2 Texcoord;
void main(){
    vec2 uv = texcoord;
    uv.y = 1.0 - uv.y;
    Texcoord = uv;
    gl_Position = position;
}