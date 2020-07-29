#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "../ShadowMap.h"
class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void renderScene();
		
		ShadowMap* shadowMap;
		ofEasyCam mainCam;
		ofShader previewShader;
		ofVboMesh mesh;
};
