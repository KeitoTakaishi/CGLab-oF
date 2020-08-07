#version 410

uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec2 texcoord;

out vec2 texCoordVarying;

void main()
{
    
    texCoordVarying = vec2(texcoord.x, 1.0 - texcoord.y);
    gl_Position = modelViewProjectionMatrix * position;
}
