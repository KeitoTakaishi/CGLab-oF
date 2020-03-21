#pragma once

#include "ofMain.h"
#define num 100
class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
		void createFrameBuffer();
		
		GLuint gBuffer, depthBuffer;
		GLuint gPosition, gNormal, gAlbedo;
		ofShader deferredShader, renderPath;
		ofEasyCam cam;
		ofVboMesh quad;


		//model pos
		ofVec3f pos[num];

};
