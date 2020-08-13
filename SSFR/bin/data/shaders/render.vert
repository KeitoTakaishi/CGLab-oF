#version 400
uniform mat4 modelViewProjectionMatrix;
uniform int type;//RenderTexture of Geometry
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;


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
    }else if(type == 2){
        //Geometry(Room)
        //gl_Position = model * vec4(position, 1.0);
        gl_Position = proj * view * model * vec4(position, 1.0);
        vTexCoord = texcoord;

        vec3 ref = position;
        ref.y *= -1.0;
        vNormal   = (vec4((ref), 0.0)).xyz;
    }else if(type == 1){
         //Geometry
        gl_Position = proj *  view * model * vec4(position, 1.0);
        vTexCoord = texcoord;
    }
}