#version 440

struct Particle {
    vec4 pos;
    vec4 vel;
    vec4 col;
};


layout(std430, binding=0) buffer particle {
    Particle paritcleDataBuffer[];
};


uniform mat4 modelViewProjectionMatrix;
uniform float time;

in vec4 position;
in vec4 color;

out vec4 vColor;

void main() {
    vec3 pos = paritcleDataBuffer[gl_VertexID].pos.xyz;
    float age = paritcleDataBuffer[gl_VertexID].pos.w;
    gl_Position = modelViewProjectionMatrix * vec4(pos, 1.0);
    //vColor = vec4(mod(time * 0.3, 1.0), 0.5, age, 1.0);
    //vColor = vec4(p[gl_VertexID].col.xyz, 1.0);
    vColor = paritcleDataBuffer[gl_VertexID].col;
    gl_PointSize = 1.0f;
}
