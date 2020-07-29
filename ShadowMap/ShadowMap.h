#pragma once

#include "ofMain.h"
class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();
	void initFbo();
	void setVirtualCam();
	void beginDepth();
	void endDepth();
	void begin(ofCamera& mainCam);
	void end(ofCamera& mainCam);
	void setPosition(ofVec3f pos) { virtualCam.setGlobalPosition(pos); }
	void lookAt(ofVec3f pos) { virtualCam.lookAt(pos); }
	void debugLight();
	
	ofFbo fbo;
	ofEasyCam virtualCam;
	ofShader shadowShader;
	ofMatrix4x4 biasMatrix;
	float nearClip;
	float farClip;

};

