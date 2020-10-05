#pragma once

#include "ofMain.h"
#include "SSFR.h"
#include "FluidSolver.h"
#include "CubeMap.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
		void initGUI();

		ofMatrix4x4 getViewMatrix(ofVec3f eye, ofVec3f up, ofVec3f center);
		ofMatrix4x4 getProjMatrix(float fov, float aspect, float n, float f);
		
		SSFR* ssfr;
		FluidSolver* solver;
		CubeMap cubeMap;
		ofImage img[6];
		
		ofEasyCam cam;
		ofBoxPrimitive room;
		ofBoxPrimitive wireBox;
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


		//Timer
		int frameCounter = 0;
		double time;
		clock_t strat;
};