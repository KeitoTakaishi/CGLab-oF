#pragma once

#include "ofMain.h"
#include "../SwapBuffer.h"
#include "../ofxDebugGui.h"
#include "ofxGui.h"
class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);	

		void updateAdv();
		void interactionForce();
		void updateDivergence();
		void updatePressure();
		void updateVel();

		ofxDebugGui* debuggui;

		SwapBuffer* swapBuffer;
		//SwapBuffer* velocity;
		//SwapBuffer* pressure;

		ofImage tex;
		ofShader resultShader;
		ofShader advectionShader;
		ofShader interactionForceShader;
		ofShader divergenceShader;
		ofShader pressureShader;
		ofShader velShader;

		//ofPlanePrimitive plane;
		ofVboMesh mesh;
		int texRes = 512;
		float dt = 0.016;
		ofVec2f meshSize = ofVec2f(512, 512);
		ofEasyCam cam;
		bool isClicked = 0;
		
		ofVec2f curCursor, preCursor;
		ofVec2f mouseVel;


		ofxPanel gui;
		ofParameter<float> viscosity;
		ofParameter<float> area;
};
