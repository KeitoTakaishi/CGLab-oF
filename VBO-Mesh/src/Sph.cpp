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
    //particle数は平方かつ、2のべき乗
    texRes = (int)ceil(sqrt(particleNum));
    cout << "tesRes : " << texRes << endl;
    pingPong.allocate(texRes, texRes, GL_RGB32F, 3);
    
    
    int ch = 3;
    float* pos;
    float* vel;
    //float* force;
    float* density;
    pos = new float [texRes * texRes * ch];
    vel = new float [texRes * texRes * ch];
    //force = new float [texRes * texRes * ch];
    density = new float [texRes * texRes * ch];
   
    for(int y = 0; y < texRes; y++){
        for(int x = 0; x < texRes; x++){
            auto i = x + texRes * y;
            pos[i * ch] = x * Params::cellSize.x;
            pos[i * ch + 1] = y * Params::cellSize.y;
            pos[i * ch + 2] = 0.0;
        }
    }
    pingPong.src->getTexture(0).loadData(pos, texRes, texRes, GL_RGB);
    delete[] pos;
    
    
    for(int y = 0; y < texRes; y++){
        for(int x = 0; x < texRes; x++){
            auto i = x + texRes * y;
            vel[i * ch + 0] = 0.0;
            vel[i * ch + 1] = 0.0;
            vel[i * ch + 2] = 0.0;
        }
    }
    pingPong.src->getTexture(1).loadData(vel, texRes, texRes, GL_RGB);
    delete[] vel;
    
//    for(int y = 0; y < texRes; y++){
//        for(int x = 0; x < texRes; x++){
//            auto i = x + texRes * y;
//            force[i * ch + 0] = 0.0;
//            force[i * ch + 1] = 0.0;
//            force[i * ch + 2] = 0.0;
//        }
//    }
//    pingPong.src->getTexture(2).loadData(force, texRes, texRes, GL_RGB);
//    delete[] force;
    
    
    
    for(int y = 0; y < texRes; y++){
        for(int x = 0; x < texRes; x++){
            auto i = x + texRes * y;
            density[i * ch + 0] = 0.0;
            density[i * ch + 1] = 0.0;
            density[i * ch + 2] = 0.0;
        }
    }
    pingPong.src->getTexture(2).loadData(density, texRes, texRes, GL_RGB);
    delete[] density;
    
    
    //vbo-init
    particle.setMode(OF_PRIMITIVE_POINTS);
    for(int y = 0; y < texRes; y++){
        for(int x = 0; x < texRes; x++){
            auto index = x + texRes * y;
            //cout << x << ", " << y << endl;
            particle.addVertex(ofVec3f(x * 2.0, y * 2.0, 0.0));
            particle.addTexCoord(ofVec2f(x, y));
            particle.addColor(ofFloatColor(1.0, 1.0, 1.0));
        }
    }
    

    //shader
    calcDensityShader.load("shaders/calcDensity");
    calcForceShader.load("shaders/calcForce");
    renderShader.load("shaders/render");
}



Sph::~Sph(){
    
}


void Sph::calcDensity(){
    pingPong.dst->begin();
    ofClear(0);
    calcDensityShader.begin();
    pingPong.src->activateAllDrawBuffers();
    pingPong.dst->activateAllDrawBuffers();
    calcDensityShader.setUniformTexture("posData",      pingPong.src->getTexture(0), 0);
    calcDensityShader.setUniformTexture("velData",      pingPong.src->getTexture(1), 1);
    calcDensityShader.setUniformTexture("densityData",  pingPong.src->getTexture(2), 2);
    calcDensityShader.setUniform1f("texRes", float(texRes));
    calcDensityShader.setUniform1f("h2", Params::h * Params::h);
    calcDensityShader.setUniform1f("densityCoeff", Params::densityCoeff);
    pingPong.src->draw(0.0, 0.0);
    calcDensityShader.end();
    pingPong.dst->end();
    pingPong.swap();
}


void Sph::calcForce(){
    pingPong.dst->begin();//fbo------
    ofClear(0);
    calcForceShader.begin();//shader------
    pingPong.src->activateAllDrawBuffers();
    pingPong.dst->activateAllDrawBuffers();
    calcForceShader.setUniformTexture("posData",      pingPong.src->getTexture(0), 0);
    calcForceShader.setUniformTexture("velData",      pingPong.src->getTexture(1), 1);
    calcForceShader.setUniformTexture("densityData",  pingPong.src->getTexture(2), 2);
    //values
    calcForceShader.setUniform1f("texRes", float(texRes));
    calcForceShader.setUniform1f("h", Params::h);
    calcForceShader.setUniform1f("gradPressureCoeff", Params::gradPressureCoeff);
    calcForceShader.setUniform1f("lapViscosityCoeff", Params::lapViscosityCoeff);
    
    pingPong.src->draw(0.0, 0.0);
    
    calcForceShader.end();//shader------
    pingPong.dst->end();//fbo------
    pingPong.swap();
}

void Sph::timeStep(){
    calcDensity();
    calcForce();
}


void Sph::preview(){
    renderShader.begin();
    renderShader.setUniform2f("screen", 100.0, 100.0);
    renderShader.setUniformTexture("posTex", pingPong.dst->getTexture(0), 0);
    particle.drawVertices();
    renderShader.end();
}
