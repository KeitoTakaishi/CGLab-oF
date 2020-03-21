#version 150
precision mediump float;

uniform sampler2DRect tex0;
uniform sampler2DRect tex1;
uniform vec2 screen;

in vec4 vColor;
in vec2 vTexCoord;
in vec2 vVelCoord;
in vec4 vPosition;

out vec4 vFragColor;


void main() {
    vec2 uv  = vTexCoord;
    //vFragColor = vec4(uv.x/screen.x, uv.y/screen.y, 1.0, 1.0);
    uv = uv-texture(tex1, vVelCoord).rg;
    vFragColor = texture(tex0, uv);
}