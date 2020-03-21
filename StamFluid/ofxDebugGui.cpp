#include "ofxDebugGui.h"
ofxDebugGui::ofxDebugGui()
{
	previewShader.load("shaders/preview");
}


ofxDebugGui::~ofxDebugGui()
{

}

void ofxDebugGui::load() {
	previewShader.load("shaders/preview");
}
