#version 440
const int BLOCK_SIZE = 256;
//--------------------------
//uniforms
uniform float gradPressWeight;
uniform float lapViscWeight;
uniform float h;
uniform int numParticle;
uniform float viscCoef;//M
//--------------------------
//bufferes
struct Particle{
    vec4 pos;
    vec4 vel;
};

struct Density{
    float density;
};

struct Pressure{
    float pressure;
};

struct Force{
    vec4 force;
};
layout(std430, binding = 0) buffer particle{
    Particle particleDataBuffer[];
};

// layout(std430, binding = 1) buffer particleRW{
//     Particle particleDataRWBuffer;
// };

layout(std430, binding = 1) buffer density{
    Density densityBuffer[];
};

layout(std430, binding = 2) buffer pressure{
    Pressure pressureBuffer[];
};

layout(std430, binding = 3) buffer force{
    Force forceBuffer[];
};


//--------------------------
//functions
vec3 calcGradPress(float r, float p_press, float n_press, float n_density, vec3 diff){
    float avg_press = 0.5 * (n_press - p_press);
    return gradPressWeight * avg_press / n_density * pow((h - r), 2.0) / r * diff;   
}

vec3 calcLapVelocity(float r, vec3 p_vel, vec3 n_vel, float n_density){
    vec3 vel_diff = n_vel - p_vel;
    return lapViscWeight * vel_diff / n_density * (h - r);
}
//--------------------------
layout(local_size_x = BLOCK_SIZE, local_size_y = 1, local_size_z = 1) in;
void main(){
    uint id = gl_GlobalInvocationID.x;
    Particle p = particleDataBuffer[id];
    vec3 p_pos = p.pos.xyz;
    vec3 p_vel = p.vel.xyz;
    float p_den = densityBuffer[id].density;
    float p_press = pressureBuffer[id].pressure;
    float h_sq = h * h;

    vec3 press = vec3(0.0, 0.0, 0.0);
    vec3 visco = vec3(0.0, 0.0, 0.0);

    for(int n_id = 0; n_id < numParticle; n_id++){
        if(id == n_id) continue;

        vec3 n_pos = particleDataBuffer[n_id].pos.xyz;
        vec3 diff = n_pos - p_pos; 
        float r_sq = dot(diff, diff);


        if(r_sq < h_sq){
            float n_den = densityBuffer[n_id].density;
            float n_press = pressureBuffer[n_id].pressure;
            vec3 n_vel = particleDataBuffer[n_id].vel.xyz;
            float r = sqrt(r_sq);


            //todo pressrure
            press += calcGradPress(r, p_press, n_press, n_den, diff);
            visco += calcLapVelocity(r, p_vel, n_vel, n_den);
        } 
    }

    //todo density too much
    vec3 force = press / p_den + viscCoef * visco;
    //vec3 force = press / max(p_den, 0.0001) + viscCoef * visco;
    forceBuffer[id].force = vec4(force.x, force.y, force.z, 0.0);
}