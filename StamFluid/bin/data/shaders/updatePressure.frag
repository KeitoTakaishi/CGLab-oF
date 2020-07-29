#version 150
#extension GL_ARB_explicit_attrib_location : enable
precision mediump float;

uniform sampler2DRect vel;
uniform sampler2DRect div;
uniform sampler2DRect press;


in vec2 vTexCoord;

layout (location = 0) out vec4 vFragColor0;
layout (location = 1) out vec4 vFragColor1;
layout (location = 2) out vec4 vFragColor2;

void main(){
    vec2 uv = vTexCoord.xy;

    float x0 = texture(press, uv - vec2(1.0, 0.0)).r;
    float x1 = texture(press, uv + vec2(1.0, 0.0)).r;
    float y0 = texture(press, uv - vec2(0.0, 1.0)).r;
    float y1 = texture(press, uv + vec2(0.0, 1.0)).r;

    //vec2 delta = vec2(1.0 / 512.0, 1.0 / 512.0);
    //float d = texture(div, uv + delta * 0.5).r;
    float d = texture(div, uv).r;
    float relaxed = (x0 + x1 + y0 + y1 - d) * 0.25;
    
    vFragColor0 = vec4(texture(vel, uv).rgb, 1.0);
    vFragColor1 = vec4(texture(div, uv).rgb, 1.0);
    vFragColor2 = vec4(relaxed, relaxed, 0.0, 1.0);
}