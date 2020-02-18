#pragma once

#include "ofMain.h"
#include "pingPongBuffer.hpp"
#include "ofxGui.h"
class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
    
    
    ofShader render, updatePos;
    ofVboMesh particles;
    pingPongBuffer pingPong;
    ofEasyCam cam;
    
    ofVec3f emitterPos, prevEmitterPos;
    int particleNum, texRes;
    bool showTex;
    
    //Gui
    ofVec2f fboSize = ofVec2f(300, 300);
    ofxPanel gui;
    ofxFloatSlider alpha;
};
