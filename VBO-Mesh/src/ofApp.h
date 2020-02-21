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
    ofEasyCam cam;
    
    //Gui
    ofxPanel gui;
    ofxFloatSlider alpha;
    
    ofShader debugShader;
    ofVec2f planeSize = ofVec2f(100, 100);
    ofVec2f planeRes = ofVec2f(100, 100);
    ofPlanePrimitive plane;
    ofVboMesh vbo;
    //--------------------
    Sph* sph;
};


/*
 物理量
 1.密度(feedbacksなし)
 2.
 
 */
