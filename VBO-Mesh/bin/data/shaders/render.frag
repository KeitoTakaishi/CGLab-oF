#version 150
precision mediump float;

uniform sampler2DRect posTex;

in vec4 vColor;
in vec2 vTexCoord;
in vec4 vPosition;

out vec4 vFragColor;

void main() {
    //vFragColor = vColor;
    //vec4 col = texture(posTex, vTexCoord);
    vec4 col = vec4(1.0);
    vFragColor = col;
}
