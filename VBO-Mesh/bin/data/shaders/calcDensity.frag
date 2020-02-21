#version 150
#extension GL_ARB_explicit_attrib_location : enable
precision mediump float;


//textures
uniform sampler2DRect posData;
uniform sampler2DRect velData;
uniform sampler2DRect forceData;
uniform sampler2DRect densityData;
//values
uniform float texRes;
uniform float h2;
uniform float densityCoeff;


in vec2 vTexCoord;
layout (location = 0) out vec4 vFragColor0;
layout (location = 1) out vec4 vFragColor1;
layout (location = 2) out vec4 vFragColor2;
layout (location = 3) out vec4 vFragColor3;

void main(void){
  vec3 i_pos = texture(posData, vTexCoord).xyz;
  float density = 0.0;
  vec2 iid = vTexCoord;

  //all search
  for(float i = 0.0; i < texRes; i++){
    for(float j = 0.0; j < texRes; j++){
      vec2 jid = vec2(j, i);
      vec3 j_pos = texture(posData, jid).xyz;//target pos
      vec3 dis = j_pos - i_pos;
      float dis2 = dot(dis, dis);
      if (dis2 < h2) {
        density += densityCoeff*pow((h2 - dis2), 3.0);
      }
    }
  }

  vec3 p = texture(posData, vTexCoord).xyz;
  vec3 v = texture(velData, vTexCoord).xyz;
  vec3 f = texture(forceData, vTexCoord).xyz;
  vec3 d = texture(densityData, vTexCoord).xyz;

  vFragColor0 = vec4(vec3(p), 1.0);
  vFragColor1 = vec4(vec3(v), 1.0);
  vFragColor2 = vec4(vec3(f), 1.0);
  vFragColor3 = vec4(vec3(density/1000.0), 1.0);
}
