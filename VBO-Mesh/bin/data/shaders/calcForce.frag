#version 150
#extension GL_ARB_explicit_attrib_location : enable
precision mediump float;

uniform sampler2DRect posData;
uniform sampler2DRect velData;
uniform sampler2DRect densityData;

uniform float texRes;
uniform float h;
uniform float gradPressureCoeff;
uniform float lapViscosityCoeff;

in vec2 vTexCoord;
layout (location = 0) out vec4 vFragColor0;
layout (location = 1) out vec4 vFragColor1;
layout (location = 2) out vec4 vFragColor2;


//const
const float gamma = 7.0;
const float density0 = 1000.0;
const float dt = 0.01;
const float pressureStiffness = 200.0;
const vec3 gravity = vec3(0.0, -0.5, 0.0);
const float wallStiffness = 3000.0;

vec3[4] wall = vec3[](
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(-1.0, 0.0, 1.0),
    vec3(0.0, -1.0, 1.0)
  );

void calcForce(){

  float h2 = h*h;
  vec2 iid = vTexCoord.xy;
  vec3 i_pos = texture(posData, iid).xyz;
  vec3 i_vel = texture(velData, iid).xyz;
  float i_density = texture(densityData, iid).x;

  float press1 = pressureStiffness * max(pow(i_density/density0, gamma) - 1.0, 0.0);
  vec3 acc = vec3(0.0, 0.0, 0.0);

  //search all
  for(float i = 0.0; i < texRes; i+=1.0){
    for(float j = 0.0; j < texRes; j+=1.0){
        vec2 jid = vec2(j, i);
        vec3 j_pos = texture(posData, jid).xyz;//target_pos
        vec2 diff = vec2(j_pos.x - i_pos.x, j_pos.y - i_pos.y);
        float dis2 = diff.x * diff.x + diff.y * diff.y;


        //if (dis2 < h2 && iid.x != j && iid.y != i){
        float I = iid.x + iid.y * texRes;
        float J = jid.x + jid.y * texRes;
        if (dis2 < h2){
          //圧力項
          float j_density = texture(densityData, jid).x;
          float press2 = pressureStiffness * max(pow(j_density/density0, gamma) - 1.0, 0.0);
          float dist = sqrt(dis2);

          float avg_press = 0.5 * (press1 + press2);
          float grad_press_mag = gradPressureCoeff * (avg_press / j_density) * (h - dist) * (h - dist);
          acc.xy = acc.xy + vec2(diff.x, diff.y) * grad_press_mag;

          // 粘性項による加速度
          vec3 j_vel = texture(velData, jid).xyz;
          vec2 velDiff = vec2(j_vel.x-i_vel.x, j_vel.y-i_vel.y);
          float viscMagnitude = lapViscosityCoeff / j_density * (h - dist);
          acc.xy += viscMagnitude * velDiff.xy;
        }
      }
    }

  if(i_density < 1000.0){
    acc *= 0.0;
  }else{
    acc = vec3(acc.x/i_density, acc.y/i_density, 0.0);
  }




//あってる--------------------------------
  for (int iW = 0; iW < 4; iW+=1) {
      vec2 nor = vec2(wall[iW].x, wall[iW].y);
      // 壁に食い込んだ距離（食い込んでいない場合は 0 とする）
      float dist;
      dist = -min(i_pos.x * nor.x + i_pos.y * nor.y + wall[iW].z, 0.0);
      // 壁に食い込んだ距離に応じて反対側に加速度を加える
      acc.x += dist * wallStiffness * nor.x;
      acc.y += dist * wallStiffness * nor.y;
  }




  acc += gravity;
  i_vel += acc * dt;
  i_pos += i_vel * dt;


  vFragColor0 = vec4(vec3(i_pos), 1.0);
  vFragColor1 = vec4(vec3(i_vel), 1.0);
  vFragColor2 = vec4(vec3(0.0), 1.0);
}


void main(void){
  calcForce();
}
