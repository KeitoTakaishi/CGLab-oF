#version 440

#define BLOCK_SIZE 256

struct Buffer1{
    float data;
};

layout(std430, binding = 0) buffer _buffer1{
    Buffer1 buffer1[];
};

float random(vec2 n) {
    return fract(sin(dot(n.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

layout(local_size_x = BLOCK_SIZE, local_size_y = 1, local_size_z = 1) in;
void main(){
    uint id = gl_GlobalInvocationID.x;
    Buffer1 _buffer = buffer1[id];
    _buffer.data = fract(_buffer.data + 0.2 * random(vec2(id , id)));
    //_buffer.data = 1.0;
    buffer1[id]  = _buffer;
}