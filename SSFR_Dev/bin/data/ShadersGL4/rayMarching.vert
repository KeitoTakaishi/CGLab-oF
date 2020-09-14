#version 400

in vec3 position;
in vec2 texcoord;
in vec3 normal;

out vec2 v_texcoord;

uniform mat4 modelViewProjectionMatrix;
void main()
{
    v_texcoord = texcoord;
    gl_Position = modelViewProjectionMatrix * vec4(position, 1.0);
}