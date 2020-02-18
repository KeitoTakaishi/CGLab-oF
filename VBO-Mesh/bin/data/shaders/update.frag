#version 150
precision mediump float;
uniform sampler2DRect posData;
uniform sampler2DRect velData;
uniform sampler2DRect forceData;
uniform sampler2DRect densityData;

uniform vec2 dTexRes;
uniform vec2 texRes;
uniform float densityCoeff;

uniform float h2;

in vec2 vTexCoord;

out vec4 vFragColor0;
//out vec4 vFragColor1;

void main(void){
  //vFragColor0 = pos;
  //float phi = value + pos.x * pos.y;
  //vFragColor0.g = sin(value + pos.x * pos.y);
  //vFragColor1 = vec4(1.0, 0.0, 0.0, 1.0);
}


void calcDensity(){
  vec3 pos = texture(posData, vTexCoord).xyz;
  vec3 vel = texture(velData, vTexCoord).xyz;
  vec3 force = texture(forceData, vTexCoord).xyz;
  float density = texture(densityData, vTexCoord).x;

  float _density = 0.0;

  vec2 cellID; //今自分がどのcellに属しているか




  //周囲の探索(今回は全探索)
  //texelについてと配列についての調査を行う
  for(float i = 0.0; i < texRes.y; i+=1.0){
    for(float j = 0.0; j < texRes.x; j+=1.0){
        vec2 uv = vec2(j * dTexRes.x, i * dTexRes.y);
        vec3 p = texture(posData, uv).xyz;
        vec3 dis = pos - p;
        float dis2 = dot(dis, dis);
        if (dis2 < h2) {
          _density += densityCoeff*pow((h2 - dis2), 3.0);
        }
    }
  }
}
