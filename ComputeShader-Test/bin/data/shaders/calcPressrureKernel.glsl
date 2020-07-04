#version 440
const int BLOC_SIZE = 256;

//-----------------------------
//uniforms
uniform float h;
uniform float B;
uniform float defaultDensity;
//-----------------------------
//bufferes
struct Density{
    float density;
};

struct Pressure{
    float pressure;
};

layout(std430, binding = 0) buffer density{
    Density densityBufffer[];
};

layout(std430, binding = 1) buffer pressure{
    Pressure pressureBuffer[];
};
//-----------------------------

//tait方程式
float calcPressure(float density){
    float gamma = 7.0;
    return B * max(pow(density / defaultDensity, gamma) - 1.0, 0.0); 
}

layout(local_size_x = BLOC_SIZE, local_size_y = 1, local_size_Z = 1) in;
void main(){
    uint id = gl_GlobalInvocationID.x;
    float p_density = densityBufffer[id].density;
    float p_pressure = calcPressure(p_density);
    pressureBuffer[id].pressure = p_pressure;
    //pressureBuffer[id].pressure = max(p_pressure, 0.1);
}