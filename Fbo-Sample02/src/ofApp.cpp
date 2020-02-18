#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(false);
    ofSetBackgroundColor(0);
    //ofEnableBlendMode(OF_BLENDMODE_ADD);
    cam.setupPerspective();
    
    particleNum = 512 * 512;
    texRes = (int)ceil(sqrt(particleNum));
    
    
    //shader load
    //render.load("shaders/render");
    updatePos.load("shaders/pass.vert", "shaders/update.frag");
    render.load("shaders/render");
    
    //vbo
    //ofDisableArbTexを使用していないのでがてテクセルが正規化されていない
    for(int y = 0; y < texRes; y++){
        for(int x = 0; x < texRes; x++){
            auto index = x + texRes * y;
            if(index < particleNum){
                particles.addVertex(ofVec3f(0.0, 0.0, 0.0));
                particles.addTexCoord(ofVec2f(x, y));
                particles.addColor(ofFloatColor(1.0, 1.0, 1.0));
            }
        }
    }
    
    //pingPongBufferの生成
    //colorBufferを用意、pos, vel
    pingPong.allocate(texRes, texRes, GL_RGBA32F, 1);
    
    float* posDataArr = new float[texRes * texRes * 4];
    for (int x = 0; x < texRes; x++){
        for (int y = 0; y < texRes; y++){
            int i = texRes * y + x;
            posDataArr[i*4 + 0] = (float)x/(float)texRes;
            posDataArr[i*4 + 1] = 0.0;
            posDataArr[i*4 + 2] = (float)y/(float)texRes;
            posDataArr[i*4 + 3] = 1.0;
        }
    }
    
    //fboと紐付けを行う, fbo内部にtexture
    //positionData
    pingPong.src->getTexture(0).loadData(posDataArr, texRes, texRes, GL_RGBA);
    delete [] posDataArr;

    
    //gui
    gui.setup();
    gui.add(alpha.setup("value", 0.0, 0.0, 1.0));

}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    float time = ofGetElapsedTimef();
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
}

//--------------------------------------------------------------
void ofApp::draw(){
    pingPong.src->draw(ofGetWidth() - fboSize.x, 0.0, fboSize.x, fboSize.y);
    pingPong.dst->draw(ofGetWidth() - fboSize.x, fboSize.y, fboSize.x, fboSize.y);
    
    gui.draw();
    ofDrawBitmapString("particleNum : " + to_string(particleNum), 10.0, 80.0);
    
    cam.begin();
    render.begin();
    render.setUniformTexture("posTex", pingPong.dst->getTexture(0), 0);
    render.setUniform2f("screen", ofGetWidth(), ofGetHeight());
    particles.drawVertices();
    render.end();
    cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
