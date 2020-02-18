#version 150
precision mediump float;
uniform sampler2DRect posData;
uniform sampler2DRect velData;
uniform float value;

in vec2 vTexCoord;

out vec4 vFragColor0;
//out vec4 vFragColor1;

void main(void){
  vec4 pos = texture(posData, vTexCoord);
  vFragColor0 = pos;

  float phi = value + pos.x * pos.y;
  vFragColor0.g = sin(value + pos.x * pos.y);

  //vFragColor1 = vec4(1.0, 0.0, 0.0, 1.0);
}
