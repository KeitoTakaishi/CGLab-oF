#pragma once

#include "ofMain.h"
#include "../Sph.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);

		Sph *sph;
};
