#version 150
#extension GL_ARB_explicit_attrib_location : enable
precision mediump float;
uniform sampler2DRect tex0;
uniform sampler2DRect tex1;
uniform sampler2DRect tex2;
uniform float t;
in vec2 vTexCoord;

layout (location = 0) out vec4 vFragColor0;
layout (location = 1) out vec4 vFragColor1;
layout (location = 2) out vec4 vFragColor2;

void main(){
  //vec4 col0 = vec4(0.0, 1.0, 0.0, 1.0);
  vec4 col0 = texture(tex0, vTexCoord);
  vec4 col1 = texture(tex1, vTexCoord);
  vec4 col2 = texture(tex2, vTexCoord);


  //col0 = vec4(1.0, 0.0, 0.0, 1.0);
  col1 = vec4(0.0, 1.0, 0.0, 1.0);
  col2 = vec4(0.0, 0.0, 1.0, 1.0);


  col0.r = fract(col0.r + 0.1);
  vFragColor0 = col0;
  vFragColor1 = col1;
  vFragColor2 = col2;
}
