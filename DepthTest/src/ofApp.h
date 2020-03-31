#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void render();


		ofFbo fbo;
		ofEasyCam cam;
		ofSpherePrimitive sphere[100];
		
		ofxPanel gui;
		//ofParameter<bool> isPerspective;
		ofParameter<float> nearClip;
		ofParameter<float> farClip;

		ofShader shader;
		ofVboMesh mesh;
};
