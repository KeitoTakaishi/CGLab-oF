#version 150
uniform mat4 modelViewProjectionMatrix;
uniform int type;//RenderTexture of Geometry
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform vec3 _lightDir;

in vec3 position;
in vec3 normal;
in vec2 texcoord;

out vec2 vTexCoord;
out vec3 vNormal;

void main(){
    if(type == 0){
        //RenderTexture
        gl_Position =  vec4(position, 1.0);
        vTexCoord = texcoord;
    }else{
        //Geometry
        gl_Position = proj * view * model * vec4(position, 1.0);
        vTexCoord = texcoord;

        vec3 ref = position;
        ref.y *= -1.0;
        vNormal   = (vec4((ref), 0.0)).xyz;
        //lightDir_screen = proj * view * model * vec4(_lightDir, 1.0);
    }
}