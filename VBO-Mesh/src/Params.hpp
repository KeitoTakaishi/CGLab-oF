//
//  Params.hpp
//  VBO-Mesh
//
//  Created by takaishi keito on 2020/02/18.
//

#ifndef Params_hpp
#define Params_hpp

#include <iostream>

namespace Params{
    struct int2{
    public:
        int x;
        int y;
    };
    
    
    const float pi_f = 3.141592653589793f;
    const int particleNum = 2048;
    const int maxPArticleNumInCell = 1024;



    //物理量
    const ofVec2f gravity = { 0.0f, -0.5f}; // 重力
    const float density0 = 1000.0f; // 流体の密度
    const float pressureStiffness = 200.0f; // 圧力を求める Tait 方程式の係数(初期密度 / (音速*音速) * gamma)
    const float viscosity = 0.4f; // 粘性係数(小さい方がサラサラしている)


    //sph
    const float initPSpacing = 0.0055f;
    const float particleMass = 0.00035f;
    const float deltaT = 0.01f;
    
    //------Cell
    const float simRange[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
    const int2 numCell = {
        (int)(ceilf(simRange[1] - simRange[0]) / initPSpacing),
        (int)(ceilf(simRange[3] - simRange[2]) / initPSpacing)
    };
    const ofVec2f cellSize = {
        initPSpacing,
        initPSpacing
    };
    
    //-------kernel係数
    const float h = 2.5f * cellSize.x;

}
#endif /* Params_hpp */
