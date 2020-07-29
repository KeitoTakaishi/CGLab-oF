#version 150
precision mediump float;

uniform sampler2DRect tex0;
uniform sampler2DRect velTex;
uniform sampler2DRect divTex;
uniform vec2 screen;
uniform float dt;

in vec4 vColor;
in vec2 vTexCoord;
in vec2 vVelCoord;
in vec4 vPosition;

out vec4 vFragColor;


void main() {
    vec2 uv  = vTexCoord;
    //vFragColor = vec4(uv.x/screen.x, uv.y/screen.y, 1.0, 1.0);
    

    vec2 vuv = vVelCoord;
    vuv += vec2(0.5, 0.5);
    vec2 vel = texture(velTex, vuv).rg;
    uv = uv - vel * dt;
   
    vec4 col = texture(tex0, uv - vel);
    float div = texture(divTex, vVelCoord).r;

    
    //vFragColor = col;
    //vFragColor = vec4(vel.x, vel.y, 0.0, 1.0);
    vFragColor = vec4(div, 0.0, 0.0, 1.0);
}