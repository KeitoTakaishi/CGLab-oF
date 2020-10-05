#pragma once
#include "ofMain.h"
class SSFR
{
public:
	SSFR();
	~SSFR();

	void init();
	void createFbo();
	void loadShader();

	void calcDepthBegin();
	void calcDepthEnd();

	void blurBegin();
	void blurEnd();



	ofShader calcDepthPass;
	ofShader blurPass;
	ofShader calcNormalPass;
	ofShader calcThicknessPass;
	ofShader renderSSFRPass;
	ofShader cubeMapPass;
	ofShader rayMarchingPass;

	ofFbo depthFbo;
	ofFbo blur1Fbo;
	ofFbo blur2Fbo;
	ofFbo normalFbo;
	ofFbo thicknessFbo;
	ofFbo renderFbo;
	ofFbo rayMarching;
};

