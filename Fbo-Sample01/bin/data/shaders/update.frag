#version 150
precision mediump float;

uniform sampler2DRect data;
uniform float time;
in vec2 vTexCoord;
out vec4 vFragColor0;

void main(){
  vec4 col = texture(data, vTexCoord);
  vFragColor0 = col * time;
}
