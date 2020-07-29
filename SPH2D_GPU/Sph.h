#pragma once

#include"ofMain.h"
#include"pingBongBuffer.h"

class Sph
{
public:
	Sph();
	Sph(int particleNum);
	~Sph();
	
	
	void initBuf();
	void initPosBuf();
	void initVelBuf();
	void initDensityBuf();

	void calcDensity();
	void calcForce();
	void timeStep();
	void preview();
	pingPongBuffer posBuffer;
	pingPongBuffer velBuffer;
	pingPongBuffer densityBuffer;

	ofShader initDensityProg, initForceProg, initPosProg;
	ofShader calcDensityProg, calcForceProg, renderProg;
	ofShader previewProg;

	bool isInit = false;

private:
	int particleNum;
	int texRes;
	ofVboMesh particle;
	ofVboMesh quad;

};

