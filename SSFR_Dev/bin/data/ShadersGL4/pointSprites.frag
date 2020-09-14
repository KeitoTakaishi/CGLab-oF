#version 400
precision highp float;

in vec3 vpos;
in vec2 vTexCoord;

//layout (location = 0) out vec4 FragColor;
out vec4 FragColor;

void main(){
    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}