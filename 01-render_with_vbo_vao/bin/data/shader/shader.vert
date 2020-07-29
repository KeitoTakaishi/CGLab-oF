#version 410

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;


out vec4 v_color;

void main()
{
    v_color = color;
    gl_Position = vec4(position.xyz, 1.0);
}