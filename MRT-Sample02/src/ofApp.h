#pragma once

#include "ofMain.h"
#include "pingPongBuffer.hpp"
#include "ofxGui.h"
class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void firstPass();
    void timeStep();
    void keyPressed(int key);
    
    pingPongBuffer pingPong;
    int res;
    ofEasyCam cam;
    
    
    //MRT
    ofShader firstShader;
    ofShader mrtShader;
    
    //for debug
    ofShader previewShader;
    ofVec2f planeSize = ofVec2f(200, 200);
    ofVec2f planeRes = ofVec2f(100, 100);
    ofPlanePrimitive plane;
    ofVboMesh vbo;
};

