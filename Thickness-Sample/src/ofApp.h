#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxAssimpModelLoader.h"
class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
		void initFbo();
		void createBox();
		//--------------------------------------
		vector<ofSpherePrimitive> spherees;
		vector<ofVec3f> poses;
		vector<ofVec3f> color;
		vector<float> radiuses;

		int num = 50;
		//--------------------------------------

		ofBoxPrimitive box;
		ofEasyCam cam;


		bool cullMode = true;
		ofxPanel gui;
		ofxToggle cull;
		ofxFloatSlider radius;
		ofxFloatSlider nearClip;
		ofxFloatSlider farClip;
		ofxFloatSlider thicknessCoef;
		ofxIntSlider mode;//back, front, thickness
		ofxVec3Slider thicknessCol;
		ofShader thichnessShader;
		ofShader renderShader;

		//ofFbo thicknessBuffer;
		ofFbo backRenderBuffer, frontRenderBuffer, renderBuffer;
		ofMesh quad;
		ofxAssimpModelLoader model;
		ofVboMesh modelMesh;
};
