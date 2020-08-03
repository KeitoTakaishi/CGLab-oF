#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "../ofxMRTViewer.h"
#include "../FluidSolver.h"
#include "../CubeMap.h"

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
		ofShader thicknessPass;
		ofShader renderPass;
		

		//particle parameters
		FluidSolver* fluidSolver;

		ofEasyCam cam;
		ofFbo depthFbo;
		ofFbo blurFbo1, blurFbo2;
		ofFbo calcNormalFbo;
		ofFbo thicknessFbo;
		ofFbo renderFbo;
		
		ofMesh quad;
		ofxFloatSlider particleSize;
		ofxFloatSlider blurScale;
		ofxFloatSlider blurDepthFallOff;
		ofxFloatSlider nearClip;
		ofxFloatSlider farClip;
		ofxFloatSlider alphaCoef;
		ofxVec3Slider albedoColor;
		ofxVec3Slider lightDir;
		ofxVec3Slider absorbK;
		ofxVec3Slider lightCoef;
		ofxIntSlider renderMode;//Thickness, Absorb, Final
		ofxPanel gui;

		ofxMRTViewer* mrtViewer;
		bool isPreview = true;


		//Light
		ofVboMesh lightMesh;
		ofSpherePrimitive pointLight;
		ofVec3f lightPos;
		
		//CubeMap
		CubeMap cubeMap;
		ofImage img[6];
};
