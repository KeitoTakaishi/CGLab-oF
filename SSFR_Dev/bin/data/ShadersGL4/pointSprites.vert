#version 400
uniform mat4 modelViewProjectionMatrix;
uniform float time;
in vec4 position;
in vec2 texcoord;



void main() {
    
    vec4 p = position;
    //float timeSpeed = 0.3;
    //p.x = snoise(vec4(p.xyz * 0.4, time * timeSpeed)) * 4.0;
    //p.y = snoise(vec4(p.xyz * 0.62, time * timeSpeed)) * 4.0;
    //p.z = snoise(vec4(p.xyz * 0.75, time * timeSpeed)) * 4.0;
    gl_Position =  p;
}