#version 150
#extension GL_ARB_explicit_attrib_location : enable
precision mediump float;
uniform sampler2DRect tex0;
uniform sampler2DRect tex1;
uniform sampler2DRect tex2;
uniform float time;

in vec2 vTexCoord;

layout (location = 0) out vec4 vFragColor0;
layout (location = 1) out vec4 vFragColor1;
layout (location = 2) out vec4 vFragColor2;


void main(){
  vec4 c0 = texture(tex0, vTexCoord);
  vec4 c1 = texture(tex1, vTexCoord);
  vec4 c2 = texture(tex2, vTexCoord);


  vFragColor0 = vec4(mod(c0.r + 1.0, 2.0), 0.0, 0.0, 1.0);
  vFragColor1 = vec4(0.0, mod(c0.r + 1.0, 2.0), 0.0, 1.0);
  vFragColor2 = vec4(0.0, 0.0, mod(c0.r + 1.0, 2.0), 1.0);
}
