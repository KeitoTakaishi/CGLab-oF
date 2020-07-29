#version 150

uniform sampler2D tex0;
in vec2 vTexCoord;

out vec4 vFragColor;

void main() {
    vFragColor = texture2D(tex0, vTexCoord);
}