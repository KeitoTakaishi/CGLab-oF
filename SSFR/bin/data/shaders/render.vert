#version 150
uniform mat4 modelViewProjectionMatrix;
uniform int type;//RenderTexture of Geometry
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
in vec3 position;
in vec2 texcoord;
out vec2 vTexCoord;

void main(){
    if(type == 0){
        //RenderTexture
        gl_Position =  vec4(position, 1.0);
        vTexCoord = texcoord;
    }else if(type == 1){
        gl_Position = proj * view * model * vec4(position, 1.0);
        vTexCoord = texcoord;
    }
}