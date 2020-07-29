#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		//void lateDrawUpdate();
		//void drawGui();

		void keyPressed(int key);


		ofShader shader;
		GLuint vao;
		GLuint vertex_vbo, color_vbo;
		ofVbo of_vao;
		bool is_ofApi;
		
};
