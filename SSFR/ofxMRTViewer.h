#ifndef ofxMRTViewer_hpp
#define ofxMRTViewer_hpp

#pragma once
#include "ofMain.h"

class ofxMRTViewer {
public:
	ofxMRTViewer(int _texNum);
	~ofxMRTViewer();
	void preview(ofFbo& _mrt);
	//normal, depth, blur
	void preview(ofFbo& _normal, ofFbo& _depth, ofFbo& _blurDepth);
	void preview(ofFbo& _normal, ofFbo& _depth, ofFbo& _blurDepth, ofFbo &_raymarch);
	ofVec2f panelSize;
private:
	int texNum;
	ofShader shader;
	vector<ofPlanePrimitive*> panels;
};

#endif