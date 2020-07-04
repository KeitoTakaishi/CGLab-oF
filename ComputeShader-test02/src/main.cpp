#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main() {
	/*
	ofSetupOpenGL(1024,768,OF_WINDOW);
	ofRunApp(new ofApp());
	*/
	ofGLWindowSettings s;
	s.setSize(1024, 768);
	s.setGLVersion(4, 5);
	ofCreateWindow(s);
	ofRunApp(new ofApp());
}
