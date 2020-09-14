#version 400
uniform mat4 modelViewProjectionMatrix;

in vec3 position;
in vec3 normal;
in vec2 texcoord;

out vec3 vPosition;
out vec2 vTexCoord;
out vec3 vNormal;
out vec3 envBoxvNormal;

void main(){
    gl_Position = modelViewProjectionMatrix * vec4(position, 1.0);
    vPosition = position;
    vTexCoord = texcoord;
    vNormal   = normalize(position);
    envBoxvNormal  = position;
}