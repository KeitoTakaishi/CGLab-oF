#pragma once

#include "ofMain.h"
#include "../SSFR.h"
#include "../FluidSolver.h"
#include "../CubeMap.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
		void initGUI();

		SSFR* ssfr;
		FluidSolver* solver;
		CubeMap cubeMap;
		ofImage img[6];
		
		ofEasyCam cam;
		ofBoxPrimitive room;
		ofSpherePrimitive container;
		ofShader shader;
		ofMesh quad;

		ofVec3f camPosition;
		ofVec3f camUp;
		float camFov;


		ofxPanel depthGui;
		ofParameter<float> camRadius;
		ofParameter<float> size;
		ofParameter<ofVec2f> clips;
		
		ofxPanel blurGui;
		ofParameter<float> blurScale;
		ofParameter<float> blurDepthFallOff;

		ofxPanel renderGui;
		ofxVec3Slider lightPos;
		ofxFloatSlider shininess;
		ofxVec3Slider absorbK;
};