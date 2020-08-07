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
		void initGBuffer();
		
		ofShader particle;
		ofShader depthPass;
		ofShader blurPass;
		ofShader calcNormalPass;
		ofShader thicknessPass;
		ofShader renderPass;
		ofShader raymarchPass;


		//particle parameters
		FluidSolver* fluidSolver;

		ofEasyCam cam;
		ofFbo depthFbo;
		ofFbo blurFbo1, blurFbo2;
		ofFbo calcNormalFbo;
		ofFbo thicknessFbo;
		ofFbo renderFbo;
		ofFbo g_buffer;//rayMarching(depth, normal)
		
		ofMesh quad;
		ofMesh quad_raymarch;
		ofxFloatSlider particleSize;
		ofxFloatSlider blurScale;
		ofxFloatSlider blurDepthFallOff;
		ofxFloatSlider nearClip;
		ofxFloatSlider farClip;
		ofxFloatSlider alphaCoef;
		
		//ofxVec3Slider lightDir;
		//ofxVec3Slider lightPos;
		ofParameter<ofVec3f> lightPos;
		ofParameter<ofFloatColor> albedoColor;
		ofParameter<ofFloatColor> ambientColor;
		ofParameter<ofFloatColor> specularColor;
		ofxVec3Slider absorbK;
		ofxVec3Slider lightCoef;
		ofxIntSlider renderMode;//Thickness, Absorb, Final
		ofxPanel gui;

		ofxMRTViewer* mrtViewer;
		bool isPreview = true;


		//Light
		ofVboMesh lightMesh;
		ofSpherePrimitive pointLight;
		//ofVec3f lightPos;
		
		//CubeMap
		CubeMap cubeMap;
		ofImage img[6];
};
