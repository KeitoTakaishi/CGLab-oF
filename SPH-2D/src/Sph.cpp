//
//  Sph.cpp
//  SPH-2D
//
//  Created by takaishi keito on 2020/02/15.
//

#include "Sph.hpp"

Sph::Sph(int num){
    cells.resize(numCell.y);
    for(int i = 0; i < numCell.x; i++){
        cells[i].resize(numCell.x);
    }
    
    for(int i = 0; i < numCell.y; i++){
        for(int j = 0; j < numCell.x; j++){
            cells[i][j] = new Cell();
        }
    }
    
    
    this->numParticles = num;
    
    pos.resize(numParticles);
    vel.resize(numParticles);
    force.resize(numParticles);
    density.resize(numParticles);
    //粒子の初期配置
    int numPAxis = (int)floorf(sqrtf((float)numParticles));
    for (int i = 0; i < this->numParticles; i++) {
        int xId = i % numPAxis;
        int yId = i / numPAxis;
        //m_pPosition[iP].x = g_initPSpacing * (float)xId;
        //m_pPosition[iP].y = g_initPSpacing * (float)yId;
        pos[i] = ofVec2f(cellSize.x * (float)xId, cellSize.y * (float)yId);
    }
}


Sph::~Sph(){
    
}

void Sph::timeStep(){
    static int cellStatus;
    //粒子を登録する
    cellStatus = updateCells();
    if (cellStatus != 0) {
        //return -1; // -1 を返して終了．
    }
    applyDensity();
    //applyForce();
    //updatePosition();
}
int Sph::updateCells(){
    for(int i = 0; i < numCell.y; i++){
        for (int j = 0; j < numCell.x ;j++) {
            cells[i][j]->clear();
        }
    }
    for (int iP = 0; iP < this->numParticles; iP++) {
        static int cellStatus;
        static int2 cellId;
        //座標情報を元にcellIDを算出する
        getCellId(pos[iP], cellId);
        cellStatus = cells[cellId.y][cellId.x]->add(iP);
        if (cellStatus != 0) {
            return -1;
        }
    }
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
//    cout << "---  : " << h * h << endl;
//    cout << "pow  : " << pow(h, 2.0) << endl;
//    cout << "powf : " << powf(h, 2.0) << endl;
    
    
    for(int iP = 0; iP < this->numParticles; iP++){
        float density = 0.0f;
        int2 cellId;
        getCellId(pos[iP], cellId);//現在注目している粒子のcell番号を特定
        //cout << "(cellIdX, cellIdY) : " << "( " << cellId.x << ", " << cellId.y << " )" << endl;
        
        for (int cellIdY = std::max(cellId.y - 1, 0); cellIdY <= std::min(cellId.y + 1, numCell.y - 1); cellIdY++) {
            for (int cellIdX = std::max(cellId.x - 1, 0); cellIdX <= std::min(cellId.x + 1, numCell.x - 1); cellIdX++) {
                //比較体表のcell内部に存在する粒子の数分だけループ
                for (int k = 0; k < cells[cellIdY][cellIdX]->getNumParticle(); k++) {
                    int jP;
                    jP = cells[cellIdY][cellIdX]->getParticleId(k);
                    static float2 diff;
                    diff.x = pos[jP].x - pos[iP].x;
                    diff.y = pos[jP].y - pos[iP].y;
                    float distSq;
                    distSq = diff.x * diff.x + diff.y * diff.y;
                    //poly6
                    //if (distSq < hSq && iP != jP) {
                    if (distSq < h2) {
                        density += densityCoeff * powf((h2 - distSq), 3.0);
                    }
                }
            }
        }
    }
}


void Sph::applyForce(){
    for(int iP = 0; iP < this->numParticles; iP++){
        const float h2 = h * h;
        static float pPressure1;
        
        float gamma = 7.0;
        pPressure1 = pressureStiffness * fmaxf(powf(density[iP] / density0, gamma) - 1.0f, 0.0f);
        
        static float2 acceleration;
        acceleration.x = 0.0f;
        acceleration.y = 0.0f;
        
        static int2 cellId;
        getCellId(pos[iP], cellId);
        
        
        //自分の近傍から探索を開始する
        for (int cellIdY = std::max(cellId.y - 1, 0); cellIdY <= std::min(cellId.y + 1, numCell.y - 1); cellIdY++) {
            for (int cellIdX = std::max(cellId.x - 1, 0); cellIdX <= std::min(cellId.x + 1, numCell.x - 1); cellIdX++) {
                for (int k = 0; k < cells[cellIdY][cellIdX]->getNumParticle(); k++) {
                    int jP;
                    jP = cells[cellIdY][cellIdX]->getParticleId(k);
                    float2 diff;
                    diff.x = pos[jP].x - pos[iP].x;
                    diff.y = pos[jP].y - pos[iP].y;
                    float distSq = diff.x * diff.x + diff.y * diff.y;
                    //自分自身は覗く
                    if (distSq < h2 && iP != jP)
                    {
                        // 圧力項による加速度
                        float pPressure2;
                        //比較対象の圧力
                        
                        pPressure2 = pressureStiffness * fmaxf(powf(density[jP] / density0, gamma) - 1.0f, 0.0f);
                        float dist;
                        dist = sqrtf(distSq);
                        float avgPressure;
                        avgPressure = 0.5f * (pPressure1 + pPressure2);
                        
                        float gradPressureMagnitude = gradPressureCoeff * avgPressure / density[jP] * (h - dist) * (h - dist);
                        //圧力は面積当たりの力
                        acceleration.x += gradPressureMagnitude * diff.x;
                        acceleration.y += gradPressureMagnitude * diff.y;
                        
                        // 粘性項による加速度
                        static float2 velDiff;
                        velDiff.x = vel[jP].x - vel[iP].x;
                        velDiff.y = vel[jP].y - vel[iP].y;
                        static float viscMagnitude;
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
    for(int iP = 0; iP < this->numParticles; iP++){
        static float2 position;
        position.x = pos[iP].x;
        position.y = pos[iP].y;
        static float2 velocity;
        velocity.x = vel[iP].x;
        velocity.y = vel[iP].y;
        static float2 acceleration;
        acceleration.x = force[iP].x;
        acceleration.y = force[iP].y;
        
        // 壁による加速度
        for (int iW = 0; iW < 4; iW++) {
            static float2 normal;
            normal.x = vPlanes[iW].x;
            normal.y = vPlanes[iW].y;
            
            // 壁に食い込んだ距離（食い込んでいない場合は 0 とする）
            static float dist;
            dist = -fmin(position.x * normal.x + position.y * normal.y + vPlanes[iW].z, 0.0f);
            
            // 壁に食い込んだ距離に応じて反対側に加速度を加える
            acceleration.x += dist * wallStiffness * normal.x;
            acceleration.y += dist * wallStiffness * normal.y;
        }
        
        
        
        // 重力による加速度
        acceleration.x += gravity.x;
        acceleration.y += gravity.y;
        
        // 速度と位置の更新
        velocity.x += dt * acceleration.x;
        velocity.y += dt * acceleration.y;
        position.x += dt * velocity.x;
        position.y += dt * velocity.y;
        
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
    
    ofScale(300.0, 300.0);
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
