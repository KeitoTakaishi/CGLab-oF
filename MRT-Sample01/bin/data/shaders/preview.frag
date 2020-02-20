#version 150
precision mediump float;


uniform sampler2DRect tex0;

in vec2 vTexCoord;
in vec4 vColor;

out vec4 outputColor;

void main(){
    outputColor = texture(tex0, vTexCoord) * 100.0;
    //outputColor = vColor;
}
