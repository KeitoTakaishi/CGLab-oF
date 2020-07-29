#version 150
#extension GL_ARB_explicit_attrib_location : enable
precision mediump float;

uniform sampler2DRect vel;
uniform sampler2DRect div;
uniform sampler2DRect press;

uniform vec2 cursor;//0.0 ~ texres
uniform vec2 cursorVel;
uniform float state;
uniform float area;
uniform float intensity;
uniform float dt;

in vec2 vTexCoord;

layout (location = 0) out vec4 vFragColor0;
layout (location = 1) out vec4 vFragColor1;
layout (location = 2) out vec4 vFragColor2;


void main(){
    vec2 uv = vTexCoord.xy;
    uv += vec2(0.5, 0.5);
    vec2 vel = texture(vel, uv).xy;    
    float dis = distance(cursor, uv);

    
    if(dis < area){
         //vel += cursorVel* intensity *100.0;
         vel += cursorVel* 1.0/512.0;
    }
    //vel += cursorVel * smoothstep( area, 0.0, length(cursor-gl_FragCoord.xy) ) * 0.16;
    

    vFragColor0 = vec4(vel.x, vel.y, 0.0, 1.0);
    vFragColor1 = vec4(0.0, 0.0, 0.0, 1.0);
    vFragColor2 = vec4(0.0, 0.0, 0.0, 1.0);
}