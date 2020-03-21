#version 150
#extension GL_ARB_explicit_attrib_location : enable
precision mediump float;

uniform sampler2DRect vel;
uniform sampler2DRect div;

in vec2 vTexCoord;

layout (location = 0) out vec4 vFragColor0;
layout (location = 1) out vec4 vFragColor1;
layout (location = 2) out vec4 vFragColor2;


void main(){
    vec2 uv = vTexCoord.xy;
    float x0 = texture(vel, uv - vec2(1.0, 0.0)).x;
    float x1 = texture(vel, uv + vec2(1.0, 0.0)).x;
    float y0 = texture(vel, uv - vec2(0.0, 1.0)).y;
    float y1 = texture(vel, uv + vec2(0.0, 1.0)).y;

    float divergence = (x1 - x0) + (y1 - y0);
    
    vFragColor0 = texture(vel, uv);
    vFragColor1 = vec4(divergence, divergence, 0.0, 1.0);    
    vFragColor2 = vec4(0.0, 0.0, 1.0, 1.0);    
}