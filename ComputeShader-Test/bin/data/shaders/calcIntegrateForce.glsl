#version 440

const int BLOCK_SIZE = 256;

//---------------------------
//uniforms
uniform float wallStiffness;
//uniform vec4 range;
uniform float dt;
//---------------------------
//bufferes
struct Particle{
    vec4 pos;
    vec4 vel;
};

struct Force{
    vec4 force;
};

layout(std430, binding = 0) buffer particle{
    Particle particleDataBuffer[];    
};

layout(std430, binding = 1) buffer particleRW{
    Particle particleDataRWBuffer[];    
};

layout(std430, binding = 2) buffer force{
    Force forceBuffer[]; 
};

layout(local_size_x = BLOCK_SIZE, local_size_y = 1, local_size_z = 1) in;
void main(){
    uint id = gl_GlobalInvocationID.x;

    vec3 pos = particleDataBuffer[id].pos.xyz;
    vec3 vel = particleDataBuffer[id].vel.xyz;
    vec3 acc = forceBuffer[id].force.xyz;

    //penalty
    //plus
    vec3 range = vec3(500.0, 500.0, 500.0);
    float dist = dot(vec4(pos, 1.0), vec4(1.0, 0.0, 0.0, range.x*0.5));
	acc += min(dist, 0) * -wallStiffness * vec3(1, 0, 0);

	dist = dot(vec4(pos, 1.0), vec4(0.0, 1.0, 0.0, range.y*0.5));
	acc += min(dist, 0) * -wallStiffness * vec3(0, 1, 0);

	dist = dot(vec4(pos, 1.0), vec4(0.0, 0.0, 1.0, range.z*0.5));
	acc += min(dist, 0) * -wallStiffness * vec3(0, 0, 1);

	//正の方向に関して
    //plus
	dist = dot(vec4(pos, 1.0), vec4(-1.0, 0.0, 0.0, range.x*0.5));
	acc += min(dist, 0) * -wallStiffness * vec3(-1, 0, 0);

	dist = dot(vec4(pos, 1.0), vec4(0.0, -1.0, 0.0, range.y*0.5));
	acc += min(dist, 0) * -wallStiffness * vec3(0, -1, 0);

	dist = dot(vec4(pos, 1.0), vec4(0.0, 0.0, -1.0, range.z*0.5));
	acc += min(dist, 0) * -wallStiffness * vec3(0, 0, -1);
    

	acc += vec3(0.0, -30.0, 0.0);
	vel += dt * acc;
	pos += dt * vel;

	particleDataRWBuffer[id].pos.xyz = pos;
	particleDataRWBuffer[id].vel.xyz = vel;
}