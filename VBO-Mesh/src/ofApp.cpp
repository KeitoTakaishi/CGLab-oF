#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(false);
    ofSetBackgroundColor(100);
    //ofEnableBlendMode(OF_BLENDMODE_ADD);
    cam.setupPerspective();
    
    particleNum = 1000;
    texRes = (int)ceil(sqrt(particleNum));
    
    
    //shader load
    //render.load("shaders/render");
    updatePos.load("shaders/pass.vert", "shaders/update.frag");
    
    //vbo
    //ofDisableArbTexを使用していないのでがてテクセルが正規化されていない
    for(int i = 0; i < texRes; i++){
        for(int j = 0; j < texRes; j++){
            auto index = j + texRes * i;
            if(index < particleNum){
                particles.addVertex(ofVec3f(0.0, 0.0, 0.0));
                particles.addTexCoord(ofVec2f(i, j));
                particles.addColor(ofFloatColor(1.0, 1.0, 1.0));
            }
        }
    }
    //pingPongBufferの生成
    //colorBufferを用意、pos, vel
    pingPong.allocate(texRes, texRes, GL_RGBA32F, 2);
    
    float* posDataArr = new float[texRes * texRes * 4];
    for (int x = 0; x < texRes; x++){
        for (int y = 0; y < texRes; y++){
            int i = texRes * y + x;
            posDataArr[i*4 + 0] = ofRandom(-1.0,1.0);
            posDataArr[i*4 + 1] = ofRandom(-1.0,1.0);
            posDataArr[i*4 + 2] = ofRandom(-1.0,1.0);
            posDataArr[i*4 + 3] = 1.0;
        }
    }
    
    float* velDataArr = new float[texRes * texRes * 4];
    for (int x = 0; x < texRes; x++){
        for (int y = 0; y < texRes; y++){
            int i = texRes * y + x;
            velDataArr[i*4 + 0] = ofRandom(-1.0,1.0);
            velDataArr[i*4 + 1] = ofRandom(-1.0,1.0);
            velDataArr[i*4 + 2] = ofRandom(-1.0,1.0);
            velDataArr[i*4 + 3] = 1.0;
        }
    }
    
    //fboと紐付けを行う, fbo内部にtexture
    //positionData
    pingPong.src->getTexture(0).loadData(posDataArr, texRes, texRes, GL_RGBA);
    pingPong.src->getTexture(1).loadData(velDataArr, texRes, texRes, GL_RGBA);
    delete [] posDataArr;
    delete [] velDataArr;
    
    
    
    //gui
    gui.setup();
    gui.add(alpha.setup("alpha", 0.0, 0.0, 1.0));
    
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    float time = ofGetElapsedTimef();
    
    //pingPong.dst->begin();
    updatePos.begin();
    updatePos.setUniform1f("value", alpha);
    updatePos.setUniformTexture("posData", pingPong.src->getTexture(0), 0);
    //updatePos.setUniformTexture("velData", pingPong.src->getTexture(1), 1);
    //pingPong.src->draw(0.0, 0.0);
    updatePos.end();
    //pingPong.dst->end();
}

//--------------------------------------------------------------
void ofApp::draw(){
    updatePos.begin();
    updatePos.setUniform1f("value",  fmod(ofGetElapsedTimef(), 1.0));
    updatePos.setUniformTexture("posData", pingPong.src->getTexture(0), 0);
    pingPong.src->draw(ofGetWidth() - fboSize.x, 0, fboSize.x, fboSize.y);
    updatePos.end();
   
    //pingPong.dst->draw(ofGetWidth() - fboSize.x, fboSize.y, fboSize.x, fboSize.y);

    gui.draw();
}
