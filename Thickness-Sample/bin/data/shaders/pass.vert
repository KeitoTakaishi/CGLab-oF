#version 150 
uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec2 texcoord;

out float depth;
out vec2 vTexCoord;

void main()
{
    
    vTexCoord = vec2(texcoord.x, 1.0 - texcoord.y);
    vec4 p = modelViewProjectionMatrix * position;
    depth = p.z / p.w;
    gl_Position = p;
}