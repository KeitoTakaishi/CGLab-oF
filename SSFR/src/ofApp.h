#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "../ofxMRTViewer.h"
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
		ofShader renderPass;
		
		ofVec3f range;
		vector<ofVec3f> vertices;
		vector<ofVec3f> vel;
		ofVboMesh vbo;
		ofEasyCam cam;
		int particleNum;
		ofFbo depthFbo;
		ofFbo blurFbo1, blurFbo2;
		ofFbo calcNormalFbo;
		ofFbo renderFbo;
		
		ofMesh quad;
		ofxFloatSlider particleSize;
		ofxFloatSlider blurScale;
		ofxFloatSlider blurDepthFallOff;
		ofxFloatSlider nearClip;
		ofxFloatSlider farClip;
		ofxPanel gui;

		ofxMRTViewer* mrtViewer;

		ofBoxPrimitive boundingBox;

		//Light
		ofSpherePrimitive pointLight;
		ofVec3f lightPos;
		
};
