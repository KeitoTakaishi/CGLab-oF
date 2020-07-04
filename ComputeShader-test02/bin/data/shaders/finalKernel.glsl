#version 440

#define BLOCK_SIZE 256

struct Particle{
    vec4 pos;
    vec4 vel;
    vec4 col;
};

struct Buffer1{
    float data;
};


layout(std430, binding = 0) buffer particle{
    Particle particleBuffer[];
};

layout(std430, binding = 1) buffer particleRW{
    Particle particleRWBuffer[];
};

layout(std430, binding = 2) buffer _buffer1{
    Buffer1 buffer1[];
};


float random(vec2 n) {
    return fract(sin(dot(n.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

//layout(local_size_x = 4, local_size_y = 1, local_size_z = 1) in;
layout(local_size_x = BLOCK_SIZE, local_size_y = 1, local_size_z = 1) in;
void main(){
    uint id = gl_GlobalInvocationID.x;
    Particle p = particleBuffer[id];
    vec3 pos = p.pos.xyz;
    vec4 col = p.col.rgba;
    if(pos.y > 250.0){
         pos.y = -250.0;
    }
    pos.y += 0.5 + 0.5 * random(vec2(id, id));

    float data = buffer1[id].data;
    col = vec4(data, data, data, 1.0);
    particleRWBuffer[id].col.rgba = col; 
    
   
    particleRWBuffer[id].pos = vec4(vec3(pos), 0.0);
    
    

    //particleBuffer[id].pos = vec4(vec3(pos), 0.0);
}