#pragma once
#include "ofMain.h"

class FluidSolver
{
public:
	FluidSolver();
	~FluidSolver();

	void init();
	void update();
	void draw();

	int particleNum;
	ofVec3f range;

	ofVboMesh particle;
	vector<ofVec3f> pos;
	vector<ofVec3f> vel;
	ofVec3f* posParticles;
};

