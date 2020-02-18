//
//  Cell.cpp
//  SPH-2D
//
//  Created by takaishi keito on 2020/02/15.
//

#include "Cell.hpp"

Cell::Cell(){
    particleIdInCell = new int[maxNumParticleIndInCell];
    this->numParticleInCell = 0;
}

Cell::~Cell(){
    delete[] particleIdInCell;
}

void Cell::clear() {
    this->numParticleInCell = 0; // セル内の粒子数をゼロにする
}   

int Cell::getNumParticle(){
    return this->numParticleInCell;
}


int Cell::getParticleId(int i){
    return particleIdInCell[i];
}

//引数には粒子のidが来る
int Cell::add(int i){
    particleIdInCell[numParticleInCell] = i;
    numParticleInCell++;
    
    if(this->numParticleInCell > maxNumParticleIndInCell){
        cout << "****Particle in cell is Out of Range ****" << endl;
        return -1;
    }
    return 0;
}

