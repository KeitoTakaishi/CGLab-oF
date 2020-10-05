#pragma once
#include "ofMain.h"
#include "ofxJson.h"

class FluidSolver
{
public:
	FluidSolver();
	~FluidSolver();

	void loadFile();
	void draw();

	int numParticle;
	
	ofVboMesh particle;
	vector<ofVec3f> pos;
	ofVec3f* posParticles;
};

