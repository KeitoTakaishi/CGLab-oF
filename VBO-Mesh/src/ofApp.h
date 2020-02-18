#pragma once

#include "ofMain.h"
#include "pingPongBuffer.hpp"
#include "ofxGui.h"
#include "Params.hpp"
#include "Sph.hpp"
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
    ofVec2f fboSize = ofVec2f(150, 150);
    ofxPanel gui;
    ofxFloatSlider alpha;
    
    
    //--------------------
    Sph* sph;
    
};


/*
 物理量
 1.密度(feedbacksなし)
 2.
 
 */
