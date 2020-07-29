#version 150
#extension GL_ARB_explicit_attrib_location : enable
precision mediump float;

uniform sampler2DRect vel;
uniform sampler2DRect div;
uniform sampler2DRect press;
uniform float viscosity;


in vec2 vTexCoord;

layout (location = 0) out vec4 vFragColor0;
layout (location = 1) out vec4 vFragColor1;
layout (location = 2) out vec4 vFragColor2;

void main(){
    vec2 uv = vTexCoord.xy;
    //uv += vec2(0.5, 0.5);
    vec2 v = texture(vel, uv).rg;
    float d = texture(div, uv).r;
    float p = texture(press, uv).r;
    
    float pi0 = texture(press, uv - vec2(1.0, 0.0)).r;
    float pi1 = texture(press, uv + vec2(1.0, 0.0)).r;
    float pj0 = texture(press, uv - vec2(0.0, 1.0)).r;
    float pj1 = texture(press, uv + vec2(0.0, 1.0)).r;


    vec4 v2 = vec4((v - (vec2(pi1, pj1) - vec2(pi0, pj0)) * 0.5), 0.0, 1.0); 
    v2 *= viscosity;
    
    vFragColor0 = v2;
    vFragColor1 = vec4(d, d, 0.0, 1.0);
    vFragColor2 = vec4(p, p, 0.0, 1.0);
}