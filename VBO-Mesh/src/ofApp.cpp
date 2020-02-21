#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetBackgroundColor(0.0);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    //ofEnableBlendMode(OF_BLENDMODE_ADD);
    cam.setupPerspective();
    sph = new Sph(Params::particleNum);
   
    
    debugShader.load("shaders/debug");
    plane.set(planeSize.x, planeSize.y, 100, 100);
    plane.mapTexCoordsFromTexture(sph->pingPong.src->getTexture(0));
    vbo = plane.getMesh();
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(to_string(ofGetFrameRate()));
    float time = ofGetElapsedTimef();
    
    if(ofGetFrameNum() % 10 == 0){
        cout << "calc" << endl;
        //sph->timeStep();
    }
    sph->timeStep();
}

//--------------------------------------------------------------
void ofApp::draw(){
    //debug--------------------
    cam.begin();
    debugShader.begin();
    for(int i = 0; i < 3; i++){
        ofMatrix4x4 modelMatrix;
        modelMatrix.translate(0.0, -planeSize.x/2.0, 0.0);
        modelMatrix.translate(ofGetWidth()/2.0-planeSize.x, ofGetHeight()/2.0-i*planeSize.x*1.25, 0);//offSet
        ofMatrix4x4 viewMatrix;
        viewMatrix = ofGetCurrentViewMatrix();
        ofMatrix4x4 projectionMatrix;
        projectionMatrix = cam.getProjectionMatrix();
        debugShader.setUniformMatrix4f("model", modelMatrix);
        debugShader.setUniformMatrix4f("view", viewMatrix);
        debugShader.setUniformMatrix4f("projection", projectionMatrix);
        debugShader.setUniformTexture("tex0", sph->pingPong.src->getTexture(i), 0);
        vbo.draw();
    }
    debugShader.end();
    cam.end();
        
    cam.begin();
    sph->preview();
    //ofDrawRectangle(ofVec2f(0.0, 0.0), 1.0, 1.0);
    cam.end();
}

void ofApp::keyPressed(int key){
    if(key == 's'){
        cout << "shader load" << endl;
        sph->calcDensityShader.load("shaders/calcDensity");
        sph->calcForceShader.load("shaders/calcForce");
        sph->renderShader.load("shaders/render");
    }else if(key == 'c'){
        sph->timeStep();
    }
}
