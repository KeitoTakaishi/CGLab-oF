#include "ofApp.h"
//--------------------------------------------------------------
void ofApp::setup(){
	loadShaders();
	initGUI();
	initFbo();
	initParticle();

}

//--------------------------------------------------------------
void ofApp::update(){
	ofSetWindowTitle(to_string(ofGetFrameRate()));
	cam.setNearClip(nearClip);
	cam.setFarClip(farClip);

	ofMatrix4x4 view = ofGetCurrentViewMatrix();
	ofMatrix4x4 invView = view.getInverse();
	ofMatrix4x4 proj = cam.getProjectionMatrix();

	/*
	//first-pass
	backDepth.begin();
	ofEnableDepthTest();
	glClearDepth(.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_GREATER);
	cam.begin();
	depthKernel.begin();
	//depthKernel.setUniform1f("NormalMode", (float)normalMode);
	depthKernel.setUniform1f("NormalMode", 0.0);
	depthKernel.setUniform1f("size", particleSize);
	depthKernel.setUniform1f("time", ofGetElapsedTimef());
	depthKernel.setUniformMatrix4f("viewMatrix", view);
	depthKernel.setUniformMatrix4f("invViewMatrix", view.getInverse());
	depthKernel.setUniformMatrix4f("projectionMatrix", proj);
	depthKernel.setUniformMatrix4f("invpProjectionMatrix", proj.getInverse());
	particle.draw();
	depthKernel.end();
	cam.end();
	backDepth.end();

	//seconds pass
	
	frontDepth.begin();
	ofEnableDepthTest();
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_LESS);
	cam.begin();
	depthKernel.begin();
	//depthKernel.setUniform1f("NormalMode", (float)normalMode);
	depthKernel.setUniform1f("NormalMode", 1.0);
	depthKernel.setUniform1f("size", particleSize);
	depthKernel.setUniform1f("time", ofGetElapsedTimef());
	depthKernel.setUniformMatrix4f("viewMatrix", view);
	depthKernel.setUniformMatrix4f("invViewMatrix", view.getInverse());
	depthKernel.setUniformMatrix4f("projectionMatrix", proj);
	depthKernel.setUniformMatrix4f("invpProjectionMatrix", proj.getInverse());
	particle.draw();
	depthKernel.end();
	cam.end();
	frontDepth.end();

	//render pass
	thickness.begin();
	thicknessKernel.begin();
	ofClear(0);
	//cam.begin();
	thicknessKernel.setUniformTexture("backFaceDepthTex", backDepth.getTexture(0), 0);
	thicknessKernel.setUniformTexture("frontFaceDepthTex", frontDepth.getTexture(0), 1);
	thicknessKernel.setUniform2f("camPrames", ofVec2f(nearClip, farClip));
	thicknessKernel.setUniform2f("screenScale", ofVec2f(ofGetWidth(), ofGetHeight()));
	thicknessKernel.setUniform1f("mode", (float)debugMode);
	thicknessKernel.setUniform1f("thicknessCoef", thicknessCoef);

	quad.draw();
	//cam.end();
	thicknessKernel.end();
	thickness.end();
	*/

	backDepth.begin();
	//ofEnableDepthTest();
	ofClear(0); 
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	//glClearDepth(.0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glDepthFunc(GL_GREATER);


	cam.begin();
	depthKernel.begin();
	//depthKernel.setUniform1f("NormalMode", (float)normalMode);
	depthKernel.setUniform1f("NormalMode", 0.0);
	depthKernel.setUniform1f("size", particleSize);
	depthKernel.setUniform1f("time", ofGetElapsedTimef());
	depthKernel.setUniformMatrix4f("viewMatrix", view);
	depthKernel.setUniformMatrix4f("invViewMatrix", view.getInverse());
	depthKernel.setUniformMatrix4f("projectionMatrix", proj);
	depthKernel.setUniformMatrix4f("invpProjectionMatrix", proj.getInverse());
	depthKernel.setUniform2f("camClips", ofVec2f(nearClip, farClip));
	particle.draw();
	depthKernel.end();
	cam.end();
	backDepth.end();

}

//--------------------------------------------------------------
void ofApp::draw(){
	/*
	ofSetBackgroundColor(0);
	thickness.draw(0.0, 0.0);
	ofDisableDepthTest();
	gui.draw();
	*/

	ofSetBackgroundColor(0);
	backDepth.draw(0.0, 0.0);
	ofDisableDepthTest();
	gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ' ') {
		loadShaders();
	}
}
//-------------------------------------------------------------
void ofApp::loadShaders() {
	depthKernel.setGeometryInputType(GL_POINTS);
	depthKernel.setGeometryOutputType(GL_TRIANGLE_STRIP);
	depthKernel.setGeometryOutputCount(4);
	depthKernel.load("shaders/depthKerenel.vert", "shaders/depthKerenel.frag", "shaders/depthKerenel.geom");
	thicknessKernel.load("shaders/thickness");
}
//--------------------------------------------------------------
void ofApp::initGUI() {
	gui.setup();
	gui.add(normalMode.setup("NormalMode", true));//true : front, false : back
	gui.add(cullMode.setup("CullingMode", true));
	gui.add(debugMode.setup("DebugMode", 0, 0, 2));
	gui.add(nearClip.setup("nearClip", 20.0, 0.1, 500.0));
	gui.add(farClip.setup("farClip", 1000.0, 100.0, 5000.0));
	gui.add(particleSize.setup("particleSize", 7.0, 0.01, 30.0));
	gui.add(thicknessCoef.setup("thicknessCoef", 1.5, 0.01, 100.0));
}
//--------------------------------------------------------------
void ofApp::initFbo() {
	ofFbo::Settings fboSetting;
	fboSetting.width = ofGetWidth();
	fboSetting.height = ofGetHeight();
	fboSetting.numColorbuffers = 1;
	fboSetting.useDepth = true;
	fboSetting.useStencil = true;
	fboSetting.depthStencilAsTexture = true;
	fboSetting.depthStencilInternalFormat;
	fboSetting.internalformat = GL_RGBA32F;
	fboSetting.wrapModeHorizontal = GL_CLAMP_TO_EDGE;
	fboSetting.wrapModeVertical = GL_CLAMP_TO_EDGE;
	fboSetting.minFilter = GL_LINEAR;
	fboSetting.maxFilter = GL_LINEAR;

	backDepth.allocate(fboSetting);
	frontDepth.allocate(fboSetting);
	thickness.allocate(fboSetting);

	quad.addVertex(ofVec3f(-1.0, -1.0));
	quad.addVertex(ofVec3f(-1.0, 1.0));
	quad.addVertex(ofVec3f(1.0, 1.0));
	quad.addVertex(ofVec3f(1.0, -1.0));

	quad.addTexCoord(ofVec2f(.0, .0));
	quad.addTexCoord(ofVec2f(.0, 1.0));
	quad.addTexCoord(ofVec2f(1.0, 1.0));
	quad.addTexCoord(ofVec2f(1.0, .0));

	quad.addIndex(0);
	quad.addIndex(1);
	quad.addIndex(2);

	quad.addIndex(2);
	quad.addIndex(3);
	quad.addIndex(0);
}
//--------------------------------------------------------------
void ofApp::initParticle() {

	particleNum = 50000;
	particle.setMode(OF_PRIMITIVE_POINTS);
	ofVec3f r = ofVec3f(150, 150, 150);
	for (int i = 0; i < particleNum; i++) {
		ofVec3f p;
		if (ofRandom(1.0) > 0.75) {
			p = ofVec3f(ofRandom(0.0, r.x), ofRandom(-r.y, r.y), ofRandom(-r.z, r.z));
		}
		else {
			p = ofVec3f(ofRandom(-r.x, 0.0), ofRandom(-r.y, r.y), ofRandom(-r.z, r.z));
		}
		
		pos.push_back(p);
		particle.addVertex(p);
	}
}