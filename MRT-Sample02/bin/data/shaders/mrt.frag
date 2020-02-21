#version 150
#extension GL_ARB_explicit_attrib_location : enable
precision mediump float;
uniform sampler2DRect tex0;
uniform sampler2DRect tex1;
uniform sampler2DRect tex2;
uniform vec3 v;

in vec2 vTexCoord;

layout (location = 0) out vec4 vFragColor0;
layout (location = 1) out vec4 vFragColor1;
layout (location = 2) out vec4 vFragColor2;


void main(){

  vec4 c0 = texture(tex0, vTexCoord);
  vec4 c1 = texture(tex1, vTexCoord);
  vec4 c2 = texture(tex2, vTexCoord);
  vFragColor0 = c0;
  vFragColor1 = c0;
  vFragColor2 = c2;
}
