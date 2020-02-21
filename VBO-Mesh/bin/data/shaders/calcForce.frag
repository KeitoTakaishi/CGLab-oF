#version 150
#extension GL_ARB_explicit_attrib_location : enable
precision mediump float;
uniform sampler2DRect posData;
uniform sampler2DRect velData;
uniform sampler2DRect forceData;
uniform sampler2DRect densityData;

uniform float texRes;
uniform float h2;
uniform float densityCoeff;
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
const float pressureStiffness = 200.0;
const float dt = 0.01;
//world
float density;
vec3 pos;
vec3 vel;
vec3 force;


void calcForce(){
  float press1;
  press1 = pressureStiffness * max(pow(density/ density0, gamma) - 1.0f, 0.0f);
  vec2 acceleration = vec2(0.0, 0.0);

  float iP = vTexCoord.x + vTexCoord.y * texRes;
  for(float i = 0.0; i < texRes; i+=1.0){
    for(float j = 0.0; j < texRes; j+=1.0){
        vec2 uv = vec2(j, i);
        vec3 p = texture(posData, uv).xyz;
        vec3 dis = pos - p;
        float dis2 = dot(dis, dis);

        float jP = i + j * texRes;

        if (dis2 < h2 && iP != jP){

          float press2 = pressureStiffness * max(pow(density[jP] / density0, gamma) - 1.0f, 0.0f);
          float dist = sqrtf(dis2);
          float avgPressure = 0.5f * (pPressure1 + pPressure2);
          float gradPressureMagnitude = gradPressureCoeff * avgPressure / density[jP] * (h - dist) * (h - dist);

          acceleration.x += gradPressureMagnitude * diff.x;
          acceleration.y += gradPressureMagnitude * diff.y;

          // 粘性項による加速度
          float2 velDiff;
          velDiff.x = vel[jP].x - vel[iP].x;
          velDiff.y = vel[jP].y - vel[iP].y;
          float viscMagnitude;
          viscMagnitude = lapViscosityCoeff / density[jP] * (h - dist);
          acceleration.x += viscMagnitude * velDiff.x;
          acceleration.y += viscMagnitude * velDiff.y;

        }

      }
    }
}


/*
void Sph::updatePosition(){
    for(int iP = 0; iP < numParticles; iP++){
        ofVec2f position, velocity, acceleration;
        position = ofVec2f(pos[iP].x, pos[iP].y);
        velocity = ofVec2f( vel[iP].x,  vel[iP].y);
        acceleration = ofVec2f( force[iP].x, force[iP].y);

        // 壁による加速度
        for (int iW = 0; iW < 4; iW++) {
            float2 normal;
            normal.x = vPlanes[iW].x;
            normal.y = vPlanes[iW].y;

            // 壁に食い込んだ距離（食い込んでいない場合は 0 とする）
            float dist;
            dist = -fmin(position.x * normal.x + position.y * normal.y + vPlanes[iW].z, 0.0f);
            // 壁に食い込んだ距離に応じて反対側に加速度を加える
            acceleration.x += dist * wallStiffness * normal.x;
            acceleration.y += dist * wallStiffness * normal.y;
        }

        // 重力による加速度

        acceleration.x += gravity.x;
        acceleration.y += gravity.y;

        // 速度と位置の更新
        velocity += acceleration * deltaT;
        position += velocity * deltaT;

        // 更新された速度と位置を配列に格納

        pos[iP].x = position.x;
        pos[iP].y = position.y;

        vel[iP].x = velocity.x;
        vel[iP].y = velocity.y;

    }
    return;
}

*/
void main(void){
  //vFragColor0 = pos;
  //float phi = value + pos.x * pos.y;
  //vFragColor0.g = sin(value + pos.x * pos.y);
  //vFragColor1 = vec4(1.0, 0.0, 0.0, 1.0);

  vec3 pos = texture(posData, vTexCoord).xyz;
  vec3 vel = texture(velData, vTexCoord).xyz;
  vec3 force = texture(forceData, vTexCoord).xyz;
  calcDensity();
  //calcForce();



  pos.x += dt*1000.0;
  if(pos.x > 1000.0){
    pos.x = 0.0;
  }


  //vFragColor0 = vec4(vec3(1.0, 0.0, 0.0), 1.0);
  //vFragColor1 = vec4(vec3(0.0, 1.0, 0.0), 1.0);
  //vFragColor2 = vec4(vec3(0.0, 0.0, 1.0), 1.0);
  vFragColor0 = vec4(vec3(pos), 1.0);
  vFragColor1 = vec4(texture(velData, vTexCoord).xyz, 1.0);
  vFragColor2 = vec4(texture(forceData, vTexCoord).xyz, 1.0);
  vFragColor3 = vec4(vec3(density), 1.0);
}
