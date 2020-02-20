#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetBackgroundColor(0);
    res = 4;
    //internal format
    pingPong.allocate(res, res, GL_RGBA32F, 3);
    
    //cout << "chech texture num" << endl;
    //cout << pingPong.src->getNumTextures() << endl;
    //cout << pingPong.dst->getNumTextures() << endl;
    
    
    int ch = 3;
    //----ColorTexture1
    float* data1;
    data1 = new float [res * res * ch];
    for(int y=0; y < res; y++){
        for(int x = 0; x < res; x++){
            auto i = (x + y * res) * ch;
            data1[i] = 0.0;
            data1[i+1] = 0.0;
            data1[i+2] = 0.0;
         }
    }
    //pixelFormat
    pingPong.src->getTexture(0).loadData(data1, res, res, GL_RGB);
    delete[] data1;
    //std::vector<float>().swap(data1);
    
    
    //---ColorTexture2
    ch = 2;
    float* data2;
    data2 = new float [res * res * ch];
    for(int y=0; y < res; y++){
        for(int x = 0; x < res; x++){
            auto i = (x + y * res) * ch;
            data2[i] = 0.0;
            data2[i+1] = 0.0;
        }
    }
    //pixelFormat
    pingPong.src->getTexture(1).loadData(data2, res, res, GL_RG);
    delete[] data2;
    
    
    
    //---ColorTexture3
    ch = 1;
    float* data3;
    data3 = new float [res * res * ch];
    for(int y=0; y < res; y++){
        for(int x = 0; x < res; x++){
            auto i = (x + y * res) * ch;
            data3[i] = 0.0;
        }
    }
    //pixelFormat
    pingPong.src->getTexture(2).loadData(data3, res, res, GL_R);
    delete[] data3;
    //std::vector<float>().swap(data3);
    
    plane.set(planeSize.x, planeSize.y, 10, 10);
    plane.mapTexCoords(0, 0, res, res);
    
    
    previewShader.load("shaders/preview");
    mrtShader.load("shaders/mrt");
}
//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
    cam.begin();
    previewShader.begin();
    
    for(int i = 0; i < 3; i++){
        ofMatrix4x4 modelMatrix;
        modelMatrix.translate(-planeSize.x + planeSize.x * i, 0.0, 0);
        modelMatrix.translate( 50.0*(i - 1), 0.0, 0);//offSet
        ofMatrix4x4 viewMatrix;
        viewMatrix = ofGetCurrentViewMatrix();
        ofMatrix4x4 projectionMatrix;
        projectionMatrix = cam.getProjectionMatrix();
        previewShader.setUniformMatrix4f("model", modelMatrix);
        previewShader.setUniformMatrix4f("view", viewMatrix);
        previewShader.setUniformMatrix4f("projection", projectionMatrix);
        previewShader.setUniformTexture("tex0", pingPong.src->getTexture(i), 0);
        plane.draw();
    }
    
    
    previewShader.end();
    cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 's'){
        mrtShader.load("shaders/mrt");
        previewShader.load("shaders/preview");
    }else if(key == ' '){
        timeStep();
    }
}
//--------------------------------------------------------------
void ofApp::timeStep(){
    //mrt------
    float t = ofGetElapsedTimef();
    
    pingPong.dst->begin();
    ofClear(0);
    mrtShader.begin();
    pingPong.src->activateAllDrawBuffers();
    pingPong.dst->activateAllDrawBuffers();
    mrtShader.setUniform1f("time", t);
    previewShader.setUniformTexture("tex0", pingPong.dst->getTexture(0), 0);
    previewShader.setUniformTexture("tex1", pingPong.dst->getTexture(1), 1);
    previewShader.setUniformTexture("tex2", pingPong.dst->getTexture(2), 2);
    pingPong.src->draw(0.0, 0.0);
    mrtShader.end();
    pingPong.dst->end();
    pingPong.swap();
    //---------
}
