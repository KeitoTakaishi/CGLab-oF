#version 150
precision mediump float;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 mvp;


in vec3 position;
in vec2 texcoord;
in vec4 color;

out vec4 vColor;
out vec2 vTexCoord;

void main() {
    vTexCoord = texcoord;
    vColor = color;
    gl_Position = projection * view * model * vec4(position, 1.0);
    //gl_Position = mvp * vec4(position, 1.0);

}
