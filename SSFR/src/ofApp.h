#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		
		ofShader shader;
		ofVboMesh vbo;
		ofEasyCam cam;
		int particleNum;
		ofFbo depthPass;
		
};
