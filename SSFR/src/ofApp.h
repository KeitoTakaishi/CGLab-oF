#pragma once

#include "ofMain.h"
#include "ofxGui.h"
class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		
		ofShader particle;
		ofShader depthPass;
		ofShader blurPass;
		ofShader calcNormalPass;
		
		vector<ofVec3f> vertices;
		ofVboMesh vbo;
		ofEasyCam cam;
		int particleNum;
		ofFbo depthFbo;
		ofFbo blurFbo1, blurFbo2;
		ofFbo calcNormalFbo;

		ofxFloatSlider blurScale;
		ofxFloatSlider blurDepthFallOff;
		ofxPanel gui;
};
