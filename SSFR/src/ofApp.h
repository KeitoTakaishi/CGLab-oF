#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "../ofxMRTViewer.h"
#include "../FluidSolver.h"
class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
		void preLoad();
		void initGUI();
		void initFbo();
		
		ofShader particle;
		ofShader depthPass;
		ofShader blurPass;
		ofShader calcNormalPass;
		ofShader renderPass;
		
		//particle parameters
		FluidSolver* fluidSolver;

		ofEasyCam cam;
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


		//Light
		ofSpherePrimitive pointLight;
		ofVec3f lightPos;
		
};
