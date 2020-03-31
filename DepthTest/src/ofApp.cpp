#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	ofDisableArbTex();
	ofFbo::Settings fboSettings;
	fboSettings.width = ofGetWidth();
	fboSettings.height = ofGetHeight();
	fboSettings.textureTarget = GL_TEXTURE_2D;
	fboSettings.internalformat = GL_R8;
	fboSettings.useDepth = true;
	fboSettings.depthStencilAsTexture = true;
	fboSettings.useStencil = true;
	fboSettings.minFilter = GL_LINEAR;
	fboSettings.maxFilter = GL_LINEAR;
	fboSettings.wrapModeHorizontal = GL_CLAMP_TO_EDGE;
	fboSettings.wrapModeVertical = GL_CLAMP_TO_EDGE;
	fbo.allocate(fboSettings);
	



	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			auto index = j + 10 * i;
			sphere[index].set(10.0, 24);
		}
	}

	//shader.load("shaders/shader");
	shader.load("shaders/pass.vert", "shaders/depthTexture.frag");
	
	gui.setup();
	gui.add(nearClip.set("nearClip", 1.0, 1.0, 600));
	gui.add(farClip.set("farClip", 600.0, 1.0, 1000));


	int res = 1.0;
	mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
	float w = 1.0;
	float h = 1.0;
	float iw = res;
	float ih = res;

	mesh.addVertex(ofVec3f(w, h, 0.0));
	mesh.addTexCoord(ofVec2f(iw, ih));

	mesh.addVertex(ofVec3f(w, -h, 0.0));
	mesh.addTexCoord(ofVec2f(iw, 0.0f));

	mesh.addVertex(ofVec3f(-w, -h, 0.0));
	mesh.addTexCoord(ofVec2f(0.0f, 0.0f));

	mesh.addVertex(ofVec3f(-w, h, 0.0));
	mesh.addTexCoord(ofVec2f(0.0f, ih));	
}

//--------------------------------------------------------------
void ofApp::update(){
	cam.setNearClip(nearClip);
	cam.setFarClip(farClip);
	
	
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	ofEnableDepthTest();
	fbo.begin();
	render();
	fbo.end();
	ofDisableDepthTest();




	if (ofGetKeyPressed(' ')) {
		fbo.draw(0.0, 0.0);
	}
	else {
		shader.begin();
		int id = fbo.getDepthTexture().getTextureData().textureID;
		cout << id << endl;
		shader.setUniformTexture("tex0", fbo.getDepthTexture(), 1);
		shader.setUniform2f("camClips", ofVec2f(nearClip, farClip));

		mesh.draw();
		shader.end();
	}
	

	gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::render() {
	cam.begin();
	ofClear(0.0);
	int _ = 10;
	float step = 50.0;
	for (int i = 0; i < _; i++) {
		for (int j = 0; j < _; j++) {
			auto index = j + 10 * i;
			ofSetColor(i*15.0, 0, j *15.0);
			sphere[index].setPosition(ofVec3f((j - _ / 2)*step, 0, (i - _ / 2)*step));
			sphere[index].draw();
		}
	}
	cam.end();
}