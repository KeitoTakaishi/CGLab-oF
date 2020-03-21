#version 330 core
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

in vec3 position;
in vec3 normal;
in vec4 color;
in vec2 texcoord;

out vec3 Position;
out vec3 Normal;
out vec4 Color;
out vec2 TexCoord;

void main(){
  Position = (view * model * vec4(position, 1.0)).xyz;
  Normal = normalize(view * model * vec4(normal, 0.0)).xyz;
  Color = color;
  TexCoord = texcoord;
  gl_Position = proj * view * model * vec4(position, 1.0);
}