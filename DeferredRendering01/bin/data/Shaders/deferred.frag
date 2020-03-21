#version 330 core
uniform sampler2D tex0;

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
//layout (location = 2) out vec4 gAlbedo; 

in vec3 Position;
in vec3 Normal;
in vec4 Color;
in vec2 TexCoord;


void main(){
  gPosition = vec4(Position, 1.0);
  gNormal = vec4(Normal, 1.0);
  //gAlbedo = texture(albedoTexture, TexCoord);
}