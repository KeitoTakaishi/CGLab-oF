//
//  Sph.cpp
//  SPH-2D
//
//  Created by takaishi keito on 2020/02/15.
//

#include "Sph.hpp"

Sph::Sph(int num){
    cout << "cellSize : " << " ( " << numCell.x << ", " << numCell.y << " ) " << endl;
    
    cells.resize(numCell.y);
    for(int i = 0; i < numCell.y; i++){
        cells[i].resize(numCell.x);
    }
    
    for(int i = 0; i < numCell.y; i++){
        for(int j = 0; j < numCell.x; j++){
            cells[i][j] = new Cell();
        }
    }
    
    
    this->numParticles = num;
    pos.resize(this->numParticles);
    vel.resize(this->numParticles);
    force.resize(this->numParticles);
    density.resize(this->numParticles);
//    pos = new float2[numParticles];
//    vel = new float2[numParticles];
//    density = new float[numParticles];
//    force = new float2[numParticles];
    //粒子の初期配置
    int numPAxis = (int)floorf(sqrtf((float)this->numParticles));
    for (int i = 0; i < this->numParticles; i++) {
        int xId = i % numPAxis;
        int yId = i / numPAxis;
        //pos[i] = ofVec2f(initPSpacing * (float)xId, initPSpacing * (float)yId);
        pos[i].x = initPSpacing * (float)xId;
        pos[i].y = initPSpacing * (float)yId;
        //pos[i] = ofVec2f(ofRandom(1.0), ofRandom(1.0));
    }
}


Sph::~Sph(){
    cells.clear();
    //pos.clear();
    //vel.clear();
    //force.clear();
    //density.clear();
}

int Sph::timeStep(){
    static int cellStatus;
    //粒子を登録する
    cellStatus = updateCells();
    if (cellStatus != 0) {
        return -1; // -1 を返して終了．
    }
    applyDensity();
    applyForce();
    updatePosition();
    
    return 0;
}

//粒子をcellに登録
int Sph::updateCells(){
    //ここまではok
    //cout << "clear particles" << endl;
    for(int i = 0; i < numCell.y; i++){
        for (int j = 0; j < numCell.x; j++) {
            cells[i][j]->clear();
        }
    }
    
    
    //cout << "add particle" << endl;
    for (int iP = 0; iP < this->numParticles; iP++) {
        static int cellStatus;
        static int2 cellId;
        //座標情報を元にcellIDを算出する
        getCellId(pos[iP], cellId);
        cellStatus = cells[cellId.y][cellId.x]->add(iP);
        
        //cout << cellId.y << ", " << cellId.x << endl;
        if (cellStatus != 0) {
            return -1;
        }
    }
    
    
    //debug
    /*
    cout << "debug" << endl;
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            for(int k = 0; k < cells[i][j]->getNumParticle(); k++){
                cout << cells[i][j]->getParticleId(k) << endl;
            }
        }
    }
    */
    
    
    return 0;
}

void Sph::getCellId(ofVec2f position, int2& cellId){
    cellId.x = (int)floorf(position.x / cellSize.x);
    if(cellId.x < 0) cellId.x = 0;
    if (cellId.x >= numCell.x) cellId.x = numCell.x - 1;
    cellId.y = (int)floorf(position.y / cellSize.y);
    if (cellId.y < 0) cellId.y = 0;
    if (cellId.y >= numCell.y) cellId.y = numCell.y - 1;
}



void Sph::applyDensity(){
    float h2 = h * h;
    for(int iP = 0; iP < numParticles; iP++){
        static float _density;
        _density = 0.0f;
        static int2 cellId;
        getCellId(pos[iP], cellId);//現在注目している粒子のcell番号を特定
        //cout <<"("<<cellId.x << ", " << cellId.y << ")" << endl;
        for (int cellIdY = std::max(cellId.y - 1, 0); cellIdY <= std::min(cellId.y + 1, numCell.y - 1); cellIdY++) {
            for (int cellIdX = std::max(cellId.x - 1, 0); cellIdX <= std::min(cellId.x + 1, numCell.x - 1); cellIdX++) {
                for (int k = 0; k < cells[cellIdY][cellIdX]->getNumParticle(); k++) {
                    //cout <<" "<<"("<<cellIdX << ", " << cellIdY << ")" << endl;
                    //cout << "  " << cells[cellIdY][cellIdX]->getNumParticle() << endl;
                    int jP = cells[cellIdY][cellIdX]->getParticleId(k);
                    //cout << "pID :" << jP << endl;
                    static float2 diff;
                    diff.x = pos[jP].x - pos[iP].x;
                    diff.y = pos[jP].y - pos[iP].y;
                    float distSq = 0.0;
                    distSq = diff.x * diff.x + diff.y * diff.y;
                    //cout << "distSq" << distSq << endl;
                    //cout << "h2 : " << h2 << endl;
                    
                    if (distSq < h2) {
                        _density += densityCoeff*powf((h2 - distSq), 3.0);
                        //cout << " 1 : " << powf((h2 - distSq), 3.0) << endl;
                        //cout << " 2 : "<< densityCoeff << endl;
                        //cout << " 3 : " << _density << endl;
                        
                    }
                }
            }
        }
        //cout << _density << endl;
        density[iP] = _density;
        //cout << "----------" << endl;
    }
}


void Sph::applyForce(){
    for(int iP = 0; iP < this->numParticles; iP++){
        const float h2 = h * h;
        float pPressure1;
        float gamma = 7.0f;
        pPressure1 = pressureStiffness * fmaxf(powf(density[iP] / density0, gamma) - 1.0f, 0.0f);
        
        ofVec2f acceleration = ofVec2f(0.0, 0.0);
        int2 cellId;
        getCellId(pos[iP], cellId);
        
        //自分の近傍から探索を開始する
        for (int cellIdY = std::max(cellId.y - 1, 0); cellIdY <= std::min(cellId.y + 1, numCell.y - 1); cellIdY++) {
            for (int cellIdX = std::max(cellId.x - 1, 0); cellIdX <= std::min(cellId.x + 1, numCell.x - 1); cellIdX++){
                for (int k = 0; k < cells[cellIdY][cellIdX]->getNumParticle(); k++) {
                    int jP = cells[cellIdY][cellIdX]->getParticleId(k);
                    ofVec2f diff = ofVec2f(pos[jP].x - pos[iP].x, pos[jP].y - pos[iP].y);
                    float distSq = diff.x * diff.x + diff.y * diff.y;
                    //自分自身は覗く
                    if (distSq < h2 && iP != jP)
                    {
                        // 圧力項による加速度
                        float pPressure2 = pressureStiffness * fmaxf(powf(density[jP] / density0, gamma) - 1.0f, 0.0f);
                        float dist = sqrtf(distSq);
                        float avgPressure = 0.5f * (pPressure1 + pPressure2);
                        float gradPressureMagnitude = gradPressureCoeff * avgPressure / density[jP] * (h - dist) * (h - dist);
                        //圧力は面積当たりの力
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
        
        force[iP].x = acceleration.x / density[iP];
        force[iP].y = acceleration.y / density[iP];
        
    }
    return;
}


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


void Sph::preview(){
    ofPushMatrix();
    ofVec2f boudary = ofVec2f(1.0, 1.0);
    ofScale(500.0, 500.0);
    ofTranslate(-boudary.x / 2.0f, -boudary.y / 2.0);
    //region
    ofSetColor(255.0);
    
    ofDrawLine(0.0, 0.0, boudary.x, 0.0);
    ofDrawLine(boudary.x, 0.0, boudary.x, boudary.y);
    ofDrawLine(boudary.x, boudary.y, 0.0, boudary.y);
    ofDrawLine(0.0, boudary.x, 0.0, 0.0);
    
    for (int i = 0; i < this->numParticles; i++) {
        ofSetColor(0.0, 255.0, 200.0);
        ofDrawCircle(pos[i].x, pos[i].y, initPSpacing/2.0);
    }
    ofPopMatrix();
}
