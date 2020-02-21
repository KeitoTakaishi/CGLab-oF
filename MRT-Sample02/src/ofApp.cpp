#include "ofApp.h"
//multi pass sample
//1pass : tex0の値を更新
//2pass : tex0の値を参照
//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(30.0);
    ofSetBackgroundColor(0);
    res = 256;
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

    float* data2;
    data2 = new float [res * res * ch];
    for(int y=0; y < res; y++){
        for(int x = 0; x < res; x++){
            auto i = (x + y * res) * ch;
            data2[i] = 0.0;
            data2[i+1] = 0.0;
            data2[i+2] = 0.0;
        }
    }
    //pixelFormat
    pingPong.src->getTexture(1).loadData(data2, res, res, GL_RGB);
    delete[] data2;
    
    
    
    //---ColorTexture3
    float* data3;
    data3 = new float [res * res * ch];
    for(int y=0; y < res; y++){
        for(int x = 0; x < res; x++){
            auto i = (x + y * res) * ch;
            data3[i] = 0.0;
            data3[i+1] = 0.0;
            data3[i+2] = 0.0;
        }
    }
    //pixelFormat
    pingPong.src->getTexture(2).loadData(data3, res, res, GL_RGB);
    delete[] data3;
    //std::vector<float>().swap(data3);
    
    plane.set(planeSize.x, planeSize.y, 10, 10);
    //plane.mapTexCoords(0, 0, res, res);
    plane.mapTexCoordsFromTexture(pingPong.src->getTexture(0));
    vbo = plane.getMesh();
    
    
    previewShader.load("shaders/preview");
    firstShader.load("shaders/firstPass");
    mrtShader.load("shaders/mrt");
    
}
//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(to_string(ofGetFrameRate()));
    timeStep();
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
        vbo.draw();
    }
    
    
    previewShader.end();
    cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 's'){
        firstShader.load("shaders/firstPass");
        mrtShader.load("shaders/mrt");
        previewShader.load("shaders/preview");
    }else if(key == ' '){
        timeStep();
    }
}
//--------------------------------------------------------------
void ofApp::timeStep(){
    //first Pass
    pingPong.dst->begin();
    ofClear(0);
    firstShader.begin();
    pingPong.src->activateAllDrawBuffers();
    firstShader.setUniformTexture("tex0", pingPong.src->getTexture(0), 0);
    firstShader.setUniformTexture("tex1", pingPong.src->getTexture(1), 1);
    firstShader.setUniformTexture("tex2", pingPong.src->getTexture(2), 2);
    firstShader.setUniform1f("t", ofGetElapsedTimef());
    pingPong.src->draw(0.0, 0.0);
    firstShader.end();
    pingPong.dst->end();
    pingPong.swap();
    
    //mrt------

    float t = ofGetElapsedTimef();
    pingPong.dst->begin();
    ofClear(0);
    mrtShader.begin();
    pingPong.src->activateAllDrawBuffers();
    mrtShader.setUniform3f("v", ofVec3f(ofRandom(1.0),ofRandom(1.0), ofRandom(1.0) ));
    mrtShader.setUniformTexture("tex0", pingPong.src->getTexture(0), 0);
    mrtShader.setUniformTexture("tex1", pingPong.src->getTexture(1), 1);
    mrtShader.setUniformTexture("tex2", pingPong.src->getTexture(2), 2);
    pingPong.src->draw(0.0, 0.0);
    mrtShader.end();
    pingPong.dst->end();
    pingPong.swap();
    //---------
}

