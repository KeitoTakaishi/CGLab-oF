#version 150
precision mediump float;

uniform sampler2DRect tex0;
uniform float t;

in vec4 vColor;
in vec2 vTexCoord;
in vec4 vPosition;

out vec4 vFragColor;

void main() {
    vFragColor = vColor;
}