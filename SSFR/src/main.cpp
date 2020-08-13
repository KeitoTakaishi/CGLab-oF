#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main() {
	//    ofSetupOpenGL(1024,768,OF_WINDOW);            // <-------- setup the GL context
	ofGLWindowSettings s;
	s.setGLVersion(4, 6);
	s.setSize(1024, 768);
	//s.setSize(800, 600);
	ofCreateWindow(s);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}