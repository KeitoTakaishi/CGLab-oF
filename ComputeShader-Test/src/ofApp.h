#pragma once

#include "ofMain.h"
#include "../SPH3D_Solver.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		struct Particle {
			ofVec4f pos;
			ofVec4f vel;
			ofVec4f col;
			//ofVec3f col;
		};

		struct ColorData {
			float col;
		};

		ofBufferObject dataBuffer, colorBuffer;

		ofShader compute, renderParticle;
		ofVboMesh particle;
		int numParticle;
		ofEasyCam cam;
		ofVec3f wallSize;


		SPH3D_Solver* solver;
};
