#include "FluidSolver.h"
FluidSolver::FluidSolver()
{
	range = ofVec3f(50.0, 25.0, 50.0);
	particleNum = 30000;
	particle.setMode(OF_PRIMITIVE_POINTS);
}

FluidSolver::~FluidSolver()
{
}

void FluidSolver::init() {
	for (int i = 0; i < particleNum; i++) {
		ofVec3f p = ofVec3f(ofRandom(-range.x, range.x), ofRandom(-range.y, range.y), ofRandom(-range.z, range.z));
		pos.push_back(p);
		vel.push_back(ofVec3f(0.0f, ofRandom(2.0f), 0.0));
		particle.addVertex(p);
	}
}


void FluidSolver::update() {
	for (int i = 0; i < particleNum; i++) {
		pos[i] -= vel[i];
		if (pos[i].y < -150.0) pos[i].y = 150.0;
		particle.setVertex(i, pos[i]);
	}
}

void FluidSolver::draw() {
	particle.draw(OF_MESH_FILL);
}