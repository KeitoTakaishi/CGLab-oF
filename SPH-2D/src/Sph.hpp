//
//  Sph.hpp
//  SPH-2D
//
//  Created by takaishi keito on 2020/02/15.
//

#ifndef Sph_hpp
#define Sph_hpp

#include <iostream>
#include "ofMain.h"
#include "Vec.hpp"
#include "Cell.hpp"
#include "Params.hpp"

class Sph{
private:
    int numParticles;
    vector<ofVec2f> pos;
    vector<ofVec2f> vel;
    vector<ofVec2f> force;
    vector<float> density;

    
    int updateCells();
    void getCellId(ofVec2f pos, int2& id);
    //void getCellId(float2 pos, int2& id);
    void applyDensity();
    void applyForce();
    void updatePosition();
public:
    Sph(int num);
    ~Sph();
    int timeStep();
    void preview();
    vector<vector<Cell*>> cells;
};
#endif /* Sph_hpp */
