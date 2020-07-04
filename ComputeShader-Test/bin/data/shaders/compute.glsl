#version 440

const int BLOCK_SIZE = 1024;

uniform vec3 wallSize;
uniform vec3 wallCenter;

struct Particle {
    vec4 pos;
    vec4 vel;
    vec4 col;
    //vec3 col;
};

struct Color{
    float col;
};

layout(std430, binding=0) buffer particle{
    Particle p[];
};

layout(std430, binding=1) buffer color{
    Color c[];
};

float random(vec2 n) {
    return fract(sin(dot(n.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

vec3 reflectWall(vec3 pos, vec3 vel) {
    vec3 ws = wallSize;
    vec3 wc = wallCenter;

    if(pos.x < wc.x - ws.x * 0.5 || pos.x > wc.x + ws.x * 0.5) vel.x *= -1.0;
    if(pos.y < wc.y - ws.y * 0.5 || pos.y > wc.y + ws.y * 0.5) vel.y *= -1.0;
    if(pos.z < wc.z - ws.z * 0.5 || pos.z > wc.z + ws.z * 0.5) vel.z *= -1.0;

    return vel;
}

layout(local_size_x = BLOCK_SIZE, local_size_y = 1, local_size_z = 1) in;
void main() {
    uint id = gl_GlobalInvocationID.x;

    // Buffer Objectからid番目のデータを取得
    Particle par = p[id];
    vec3 pos = par.pos.xyz;
    vec3 vel = par.vel.xyz;
    float age = par.pos.w;
    float maxAge = par.vel.w;
    //vec3 col = c[id].col;
    float col = c[id].col;


    age += 0.01;
    if(age > maxAge) {
        age = 0.0;
        maxAge = 2.0;
        pos = vec3(random(pos.xx) * 2.0 - 1.0, random(pos.yy) * 2.0 - 1.0, random(pos.zz) * 2.0 - 1.0);
        vel = normalize(pos) * 5.0;
    }

    // 頂点に速度を足す
    pos += vel;
    vel = reflectWall(pos, vel);

    // Buffer Objectのデータ更新
    par.pos.xyz = pos;
    par.vel.xyz = vel;
    //par.pos.w = col.r;
    par.pos.w = age;
    par.vel.w = maxAge;
    //par.col.xyzw = vec4(1.0, 0.0, 0.0, 0.0);
    
    par.col.xyz = vec3(col, col, col);
    //par.col.xyz = col;
    p[id] = par;
}
