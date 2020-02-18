#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    fboSetting.width = 100;
    fboSetting.height = 100;
    fboSetting.numColorbuffers = 1;
    fboSetting.useDepth = false;
    fboSetting.internalformat = GL_RGBA;
    fboSetting.wrapModeHorizontal = GL_CLAMP_TO_EDGE;
    fboSetting.wrapModeVertical = GL_CLAMP_TO_EDGE;
    fboSetting.minFilter = GL_NEAREST;
    fboSetting.maxFilter = GL_NEAREST;
    fbo.allocate(fboSetting);
    
    int texRes = 100;
    vector<float> data(texRes*texRes*4);
    for(int y = 0; y < texRes; y++){
        for(int x = 0; x < texRes; x++){
            auto i = x + y * texRes;
            data[i * 4 + 0] = ofRandom(1.0);
            data[i * 4 + 1] = ofRandom(1.0);
            data[i * 4 + 2] = ofRandom(1.0);
            data[i * 4 + 3] = 1.0;
        }
    }
    
    fbo.getTexture(0).loadData(data.data(), texRes, texRes, GL_RGBA);
    
    shader.load("shaders/pass.vert", "shaders/update.frag");
}

//--------------------------------------------------------------
void ofApp::update(){
}
//--------------------------------------------------------------
void ofApp::draw(){
    shader.begin();
    shader.setUniform1f("time", fmod(ofGetElapsedTimef(), 1.0));
    shader.setUniformTexture("data", fbo.getTexture(0), 0);
    fbo.draw(0.0, 0.0, 300, 300);
    shader.end();
}

