#version 150
uniform sampler2D sparkTex;
in vec2 vTexCoord;
out vec4 vFragColor;

void main() {
    vFragColor = vec4(vTexCoord.x, vTexCoord.y, 0.0, 1.0);
}