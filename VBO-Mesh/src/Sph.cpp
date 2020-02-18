//
//  Sph.cpp
//  VBO-Mesh
//
//  Created by takaishi keito on 2020/02/18.
//

#include "Sph.hpp"
Sph::Sph(int _particleNum){
    particleNum = _particleNum;
    //cout << this->particleNum << endl;
    
    //pos, vel, force, densityの4つのbufferが必要
    int texRes = (int)ceil(sqrt(particleNum));
    pingPong.allocate(texRes, texRes, GL_RGBA32F, 4);
    
    
    int ch = 3;
    float* pos;
    float* vel;
    float* force;
    float* density;
    pos = new float [texRes * texRes * ch];
    vel = new float [texRes * texRes * ch];
    force = new float [texRes * texRes * ch];
    density = new float [texRes * texRes * ch];
   
    
    for(int y = 0; y < texRes; y++){
        for(int x = 0; x < texRes; x++){
            auto i = x + texRes * y;
            pos[i * ch + 0] = 0.0;
            pos[i * ch + 1] = 0.0;
            pos[i * ch + 2] = 0.0;
        }
    }
    pingPong.src->getTexture(0).loadData(pos, texRes, texRes, GL_RGBA);
    delete[] pos;
    
    
    for(int y = 0; y < texRes; y++){
        for(int x = 0; x < texRes; x++){
            auto i = x + texRes * y;
            vel[i * ch + 0] = 0.0;
            vel[i * ch + 1] = 0.0;
            vel[i * ch + 2] = 0.0;
        }
    }
    pingPong.src->getTexture(1).loadData(vel, texRes, texRes, GL_RGBA);
    delete[] vel;
    
    for(int y = 0; y < texRes; y++){
        for(int x = 0; x < texRes; x++){
            auto i = x + texRes * y;
            force[i * ch + 0] = 0.0;
            force[i * ch + 1] = 0.0;
            force[i * ch + 2] = 0.0;
        }
    }
    pingPong.src->getTexture(2).loadData(force, texRes, texRes, GL_RGBA);
    delete[] force;
    
    
    
    for(int y = 0; y < texRes; y++){
        for(int x = 0; x < texRes; x++){
            auto i = x + texRes * y;
            density[i * ch + 0] = 0.0;
            density[i * ch + 1] = 0.0;
            density[i * ch + 2] = 0.0;
        }
    }
    pingPong.src->getTexture(2).loadData(density, texRes, texRes, GL_RGBA);
    delete[] density;
    
    
    //vbo-init
    for(int y = 0; y < texRes; y++){
        for(int x = 0; x < texRes; x++){
            auto index = x + texRes * y;
            if(index < particleNum){
                particle.addVertex(ofVec3f(0.0, 0.0, 0.0));
                particle.addTexCoord(ofVec2f(x, y));
                //particle.addColor(ofFloatColor(1.0, 1.0, 1.0));
            }
        }
    }
    
    //shader
    update.load("shaders/pass.vert", "shaders/update.frag");
    render.load("shaders/update");
    
}



Sph::~Sph(){
    
}


void Sph::timeStep(){
    //shaderを用いて更新
    
    update.begin();
    //粒子をcellへ登録
    //座標などを計算し、更新を行うh
    
    //uniform
    update.setUniform1f("h2", Params::h * Params::h);
    
    
    update.end();
    
    
}

