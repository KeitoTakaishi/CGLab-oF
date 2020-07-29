#version 150
#extension GL_ARB_explicit_attrib_location : enable
precision mediump float;

uniform sampler2DRect vel;
uniform sampler2DRect div;
uniform sampler2DRect press;
uniform float dt;

in vec2 vTexCoord;

layout (location = 0) out vec4 vFragColor0;
layout (location = 1) out vec4 vFragColor1;
layout (location = 2) out vec4 vFragColor2;

void main(){
    vec2 uv = vTexCoord.xy;
    //vec2 uv = gl_FragCoord.xy;
    //uv += vec2(0.5, 0.5);
    //vec2 delta = vec2(1.0/512.0, 1.0/512.0);
    //vec2 v = texture(vel, uv + delta * 0.5).xy;
    vec2 v = texture(vel, uv).xy;


    vec2 _uv = uv -  v * dt;
    
    vec2 result = texture(vel, _uv).xy;
    
    vFragColor0 = vec4(result.x, result.y, 0.0, 1.0);
    vFragColor1 = vec4(0.0, 0.0, 0.0, 1.0);
    vFragColor2 = vec4(0.0, 0.0, 0.0, 1.0);
}