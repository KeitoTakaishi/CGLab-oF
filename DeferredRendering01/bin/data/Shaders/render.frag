#version 330 core
uniform sampler2D gPosition;
uniform sampler2D gNormal;

in vec2 TexCoord;
out vec4 col;

void main(){
    vec4 pos = texture(gPosition, TexCoord); 
    vec4 normal = texture(gNormal, TexCoord);
    col = normal;
}