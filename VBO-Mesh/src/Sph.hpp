//
//  Sph.hpp
//  VBO-Mesh
//
//  Created by takaishi keito on 2020/02/18.
//

#ifndef Sph_hpp
#define Sph_hpp

#include <iostream>
#include "ofMain.h"
#include "pingPongBuffer.hpp"
#include "Params.hpp"

class Sph{
public:
    Sph(int _particleNum);
    ~Sph();
    void timeStep();
private:
    int particleNum;
    ofVboMesh particle;
    pingPongBuffer pingPong;
    ofShader render, update;
    //bufferValues(pingPongで保持しないといけない値)
};

#endif /* Sph_hpp */
