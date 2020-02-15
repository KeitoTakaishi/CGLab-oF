//
//  Params.hpp
//  SPH-2D
//
//  Created by takaishi keito on 2020/02/14.
//

#ifndef Params_hpp
#define Params_hpp

#include <iostream>
#include "Vec.hpp"
#include "ofMain.h"

const float pi_f = 3.141592653589793f;
const float numParticles = 4096;
const int maxNumParticleIndInCell = 1280;

//物理量
const float2 gravity = { 0.0f, -0.5f}; // 重力
const float density0 = 1000.0f; // 流体の密度
const float pressureStiffness = 20.0f; // 圧力を求める Tait 方程式の係数(初期密度 / (音速*音速) * gamma)
const float viscosity = 0.3f; // 粘性係数(小さい方がサラサラしている)

//------sph
//影響半径と初期間隔の関係について
float dt = 0.01f;
const float initPSpacing = 0.0055f;
const float particleMass = 0.00035f;

//------壁に関するparams
// シミュレーションの範囲（xMin, xMax, yMin, yMax）
const float simRange[4] = { 0.0f, 1.0f, 0.0f, 1.0f };

const float3 vPlanes[4] = {
    // 法線 x 成分，法線 y 成分，原点から直線までの距離
    {1.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},
    {-1.0f, 0.0f, 1.0f},
    {0.0f, -1.0f, 1.0f}
};
const float wallStiffness = 3000.0f; // 壁のばね定数


//------Cell
const int2 numCell = {
    (int)(ceilf(simRange[1] - simRange[0]) / initPSpacing),
    (int)(ceilf(simRange[3] - simRange[2]) / initPSpacing)
};
const float2 cellSize = {
    initPSpacing,
    initPSpacing
};

//-------kernel係数
const float h = 2.5f * cellSize.x;
const float densityCoeff = particleMass * 315.0f / (64.0f * pi_f * powf(h, 9.0f)); // 密度計算の重み3D
const float gradPressureCoeff = particleMass * (-45.0f) / (pi_f * powf(h, 6.0f)); // 圧力項計算の重み3D
const float lapViscosityCoeff = particleMass * viscosity * 45.0f / (pi_f * powf(h, 6.0f)); // 粘性項計算の係数3D

#endif /* Params_hpp */
