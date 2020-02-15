//
//  Cell.cpp
//  SPH-2D
//
//  Created by takaishi keito on 2020/02/15.
//

#include "Cell.hpp"

Cell::Cell(){
    particleId = new int[maxNumParticleIndInCell];
    numParticle = 0;
}

Cell::~Cell(){
    delete[] particleId;
}

void Cell::clear() {
    this->numParticle = 0; // セル内の粒子数をゼロにする
}   

int Cell::getNumParticle(){
    return this->numParticle;
}


int Cell::getParticleId(int i){
    return this->particleId[i];
}

//引数には粒子のidが来る
int Cell::add(int i){
    particleId[this->numParticle] = i;
    this->numParticle++;
    
    if(this->numParticle > maxNumParticleIndInCell){
        cout << "****Particle in cell is Out of Range ****" << endl;
        return -1;
    }
    return 0;
}

