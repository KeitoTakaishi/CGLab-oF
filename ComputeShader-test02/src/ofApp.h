#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void drawUI();
	
		struct Particle {
			ofVec4f pos;
			ofVec4f vel;
			ofVec4f col;
		};

		struct Buffer1 {
			float data;
		};

		ofBufferObject particleBuffer;
		ofBufferObject particleRWBuffer;
		ofBufferObject temp;
		ofBufferObject buffer1;


		ofShader firstKernel, finalKernel;
		ofShader render;

		ofVboMesh particle;
		int numParticle;
		ofEasyCam cam;
		ofVec3f wallSize;
		int BLOCK_SIZE = 256;
};
