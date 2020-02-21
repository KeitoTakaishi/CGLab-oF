#version 150
#extension GL_ARB_explicit_attrib_location : enable
precision mediump float;
uniform sampler2DRect posData;
uniform sampler2DRect velData;
uniform sampler2DRect forceData;
uniform sampler2DRect densityData;

uniform float texRes;
uniform float h;
uniform float gradPressureCoeff;
uniform float lapViscosityCoeff;

in vec2 vTexCoord;

layout (location = 0) out vec4 vFragColor0;
layout (location = 1) out vec4 vFragColor1;
layout (location = 2) out vec4 vFragColor2;
layout (location = 3) out vec4 vFragColor3;


//const
const float gamma = 7.0;
const float density0 = 1000.0;
const float dt = 0.01;
const float pressureStiffness = 200.0;
const vec3 gravity = vec3(0.0, 0.5, 0.0);
const float wallStiffness = 3000.0;
vec3[4] wall = vec3[](
    vec3(1.0f, 0.0f, 0.0f),
    vec3(0.0f, 1.0f, 0.0f),
    vec3(-1.0f, 0.0f, 1.0f),
    vec3(0.0f, -1.0f, 1.0f)
  );
//world
vec3 pos;
vec3 vel;
vec3 force;


void calcForce(){
  vec2 iid = vTexCoord;
  vec3 i_pos = texture(posData, vTexCoord).xyz;
  vec3 i_vel = texture(velData, vTexCoord).xyz;
  float density = texture(densityData, vTexCoord).x;

  float press1 = pressureStiffness * max(pow(density/ density0, gamma) - 1.0f, 0.0f);
  vec3 acceleration = vec3(0.0, 0.0, 0.0);

  //search all
  for(float i = 0.0; i < texRes; i+=1.0){
    for(float j = 0.0; j < texRes; j+=1.0){
        vec2 jid = vec2(j, i);
        vec3 j_pos = texture(posData, jid).xyz;//target_pos
        vec3 diff = j_pos - i_pos;
        float dis2 = dot(diff, diff);


      if (dis2 < (h*h) && dot(iid, jid) != 1.0){
        float press2 = pressureStiffness * max(pow(density / density0, gamma) - 1.0f, 0.0f);
        float dist = sqrt(dis2);
        float avg_press = 0.5f * (press1 + press2);
        float grad_press_mag = gradPressureCoeff * avg_press / density * (h - dist) * (h - dist);

        acceleration.x += grad_press_mag * diff.x;
        acceleration.y += grad_press_mag * diff.y;

        // 粘性項による加速度
        vec3 j_vel = texture(velData, jid).xyz;
        vec2 velDiff = vec2(j_vel.x-i_vel.x, j_vel.y-i_vel.y);

        float j_density = texture(densityData, jid).x;
        float viscMagnitude;
        viscMagnitude = lapViscosityCoeff / j_density * (h - dist);
        acceleration.x += viscMagnitude * velDiff.x;
        acceleration.y += viscMagnitude * velDiff.y;
      }
    }
  }
  for (int iW = 0; iW < 4; iW+=1) {
      vec2 normal = vec2(wall[iW].x, wall[iW].y);
      // 壁に食い込んだ距離（食い込んでいない場合は 0 とする）
      float dist;
      dist = -min(i_pos.x * normal.x + i_pos.y * normal.y + wall[iW].z, 0.0f);
      // 壁に食い込んだ距離に応じて反対側に加速度を加える
      acceleration.x += dist * wallStiffness * normal.x;
      acceleration.y += dist * wallStiffness * normal.y;
  }


  acceleration.x += gravity.x;
  acceleration.y += gravity.y;

  vec3 v = i_vel + acceleration * dt;
  vec3 p = v * dt;

  vFragColor0 = vec4(vec3(p), 1.0);
  vFragColor1 = vec4(vec3(v), 1.0);
  vFragColor2 = vec4(0.0, 0.0, 0.0, 1.0);
  vFragColor3 = vec4(0.0, 0.0, 0.0, 1.0);
}


void main(void){

  calcForce();
}
