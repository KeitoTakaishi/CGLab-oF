#pragma once

#include "ofMain.h"
#include "ofxGui.h"
class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
		void generateLookupTable();
		
		
		ofShader shader;
		ofEasyCam cam;

		ofVbo vbo;
		GLuint texID;
		//MarchingCubes Parameters
		const ofVec3f marchingSpace = ofVec3f(10.0, 10.0, 10.0);
		const ofVec3f cellNum =  ofVec3f(10, 10, 10);
		const ofVec3f cellSize = ofVec3f(marchingSpace.x / cellNum.x, marchingSpace.y / cellNum.y, marchingSpace.z / cellNum.z);
		int vertexNum;
		 

		ofxPanel gui;
		//ofxFloatSlider value;

		//debug
		ofBoxPrimitive box;
		ofTexture tex;
		//GLuint tex0;	
};
