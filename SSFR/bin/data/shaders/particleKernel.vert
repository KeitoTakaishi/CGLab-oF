#version 400

in vec3 position;
in vec2 texcoord;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform float _time;


out vec2 vTexCoord;
void main(){
    vec3 p = position;
    p = vec3(0.0, 100.0 , 0.0);
    gl_Position = proj * view * model * vec4(p, 1.);
    vTexCoord = texcoord;
}
