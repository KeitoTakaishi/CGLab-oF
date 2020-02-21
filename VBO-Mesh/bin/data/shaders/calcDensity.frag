#version 150
#extension GL_ARB_explicit_attrib_location : enable
precision mediump float;

uniform sampler2DRect posData;
uniform sampler2DRect velData;
uniform sampler2DRect densityData;
//values
uniform float texRes;
uniform float h;
uniform float densityCoeff;


in vec2 vTexCoord;
layout (location = 0) out vec4 vFragColor0;
layout (location = 1) out vec4 vFragColor1;
layout (location = 2) out vec4 vFragColor2;

void main(void){
  float h2 = h*h;
  vec2 iid = vTexCoord;
  vec3 i_pos = texture(posData, iid).xyz;
  float density = 0.0;

  //all search
  for(float i = 0.0; i < texRes; i++){
    for(float j = 0.0; j < texRes; j++){
      vec2 jid = vec2(j, i);
      vec3 j_pos = texture(posData, jid).xyz;//target pos
      //vec3 dis = j_pos - i_pos;
      vec2 dis = vec2(j_pos.x - i_pos.x, j_pos.y - i_pos.y);
      float dis2 = dis.x*dis.x + dis.y*dis.y;

      if (dis2 < h2) {
        density += densityCoeff*pow((h2 - dis2), 3.0);
      }
    }
  }

  vec3 v = texture(velData, vTexCoord).xyz;
  vFragColor0 = vec4(vec3(i_pos), 1.0);
  vFragColor1 = vec4(vec3(v), 1.0);
  vFragColor2 = vec4(vec3(density), 1.0);
}
