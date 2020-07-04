#version 440

const int BLOCK_SIZE = 256;

//--------------------------
//uniforms
uniform float densityWeight;
uniform float h;
uniform int numParticle;
//uniform float time;
//--------------------------

struct Particle{
    vec4 pos;
    vec4 vel;
};

struct Density{
    float density;
};

layout(std430, binding = 0) buffer particle{
    Particle particleDataBuffer[];
};

layout(std430, binding = 1) buffer density{
    Density densityBuffer[];
};

float calcDensity(float r_sq){
    float h_sq = h * h;
    float density = densityWeight * (h_sq - r_sq) * (h_sq - r_sq) * (h_sq - r_sq);
    return density;
}

float random(vec2 n) {
    return fract(sin(dot(n.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

layout(local_size_x = BLOCK_SIZE, local_size_y = 1, local_size_z = 1) in;
void main(){
    uint id = gl_GlobalInvocationID.x;
    Particle p = particleDataBuffer[id];
    
    float h_sq = h * h;
    vec3 p_pos = particleDataBuffer[id].pos.xyz;
    
    //Todo
    float density = 0.0 + 0.01;
    //float density = 0.0;

    //Σ
    for(uint n_id = 0; n_id < numParticle; n_id++){
        if(n_id == id) continue;

        vec3 n_pos = particleDataBuffer[n_id].pos.xyz;
        vec3 diff = n_pos - p_pos;
        float r_sq = dot(diff, diff);

        if(r_sq < h_sq){
            density += calcDensity(r_sq);
        }
    }
    densityBuffer[id].density = density;
}