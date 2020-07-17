#pragma once

#include "ofMain.h"
#include "ofxGui.h"
class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
		void loadShaders();
		void initGUI();
		void initFbo();
		void initParticle();
		

		//GUI
		ofxPanel gui;
		ofxToggle normalMode;
		ofxToggle cullMode;
		ofxIntSlider debugMode;
		ofxFloatSlider nearClip;
		ofxFloatSlider farClip;
		ofxFloatSlider particleSize;
		ofxFloatSlider thicknessCoef;

		//Fbo
		ofFbo backDepth, frontDepth, thickness;

		//Shader
		ofShader depthKernel, thicknessKernel;

		//Particle
		ofVboMesh particle;
		vector<ofVec3f> pos;
		int particleNum;

		ofEasyCam cam;
		ofMesh quad;

};
