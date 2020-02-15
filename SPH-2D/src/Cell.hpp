//
//  Cell.hpp
//  SPH-2D
//
//  Created by takaishi keito on 2020/02/15.
//

#ifndef Cell_hpp
#define Cell_hpp

#include <stdio.h>
#include "ofMain.h"
#include "Params.hpp"

class Cell{
private:
    int* particleId;
    int numParticle;
public:
    Cell();
    ~Cell();
    void clear();
    int getNumParticle();
    int getParticleId(int i);
    int add(int i);
};

#endif /* Cell_hpp */
