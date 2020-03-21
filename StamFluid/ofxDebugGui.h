#pragma once
#include "ofMain.h"
class ofxDebugGui
{
public:
	ofxDebugGui();
	~ofxDebugGui();


	ofShader getShader() {
		return this->previewShader;
	}

	void load();
private:
	ofShader previewShader;
};

