#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(30);
    ofSetBackgroundColor(0.0);
    //ofEnableBlendMode(OF_BLENDMODE_ADD);
    cam.setupPerspective();
    sph = new Sph(Params::particleNum);
   
    
    debugShader.load("shaders/debug");
    plane.set(planeSize.x, planeSize.y, 10, 10);
    plane.mapTexCoordsFromTexture(sph->pingPong.src->getTexture(0));
    vbo = plane.getMesh();
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(to_string(ofGetFrameRate()));
    float time = ofGetElapsedTimef();
    //sph->timeStep();
    /*
    alpha = fmod(time, 1.0);
    pingPong.dst->begin();
    ofClear(0);
    updatePos.begin();
    updatePos.setUniform1f("value", alpha);
    updatePos.setUniformTexture("posData", pingPong.src->getTexture(0), 0);
    //updatePos.setUniformTexture("velData", pingPong.src->getTexture(1), 1);
    pingPong.src->draw(0.0, 0.0);
    updatePos.end();
    pingPong.dst->end();
    
    pingPong.swap();
     */
    //sph->timeStep();
}

//--------------------------------------------------------------
void ofApp::draw(){
    //debug--------------------
    cam.begin();
    debugShader.begin();
    for(int i = 0; i < 4; i++){
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
        debugShader.setUniformTexture("tex" + to_string(0), sph->pingPong.src->getTexture(i), 0);
        vbo.draw();
    }
    debugShader.end();
    cam.end();
    
    /*
    cout << "--------------" << endl;
    cout << sph->pingPong.src->getTexture(0).getTextureData().textureID << endl;
    cout << sph->pingPong.src->getTexture(1).getTextureData().textureID << endl;
    cout << sph->pingPong.src->getTexture(2).getTextureData().textureID << endl;
    cout << sph->pingPong.src->getTexture(3).getTextureData().textureID << endl;
     */
    
//    cam.begin();
//    sph->preview();
//    cam.end();
}

void ofApp::keyPressed(int key){
    if(key == 's'){
        cout << "shader load" << endl;
        sph->calcDensityShader.load("shaders/calcDensity");
        //sph->calcForcShader.load("shaders/calcForce");
        //sph->renderShader().load("shaders/render");
    }else if(key == 'c'){
        sph->timeStep();
    }
}
