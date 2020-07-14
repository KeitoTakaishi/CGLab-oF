#include "ofApp.h"

//--------------------------------------------------------------
/*
1. depth
2. apply blur
3. normal
4. render
*/
//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(false);
	preLoad();
	initFbo();
	initGUI();

	fluidSolver = new FluidSolver();
	fluidSolver->init();
	mrtViewer = new ofxMRTViewer(3);
	cam.setupPerspective(true, 60.0f, nearClip, farClip);
}

//--------------------------------------------------------------
void ofApp::update(){
	ofSetWindowTitle(to_string(ofGetFrameRate()));
	cam.setNearClip(nearClip);
	cam.setFarClip(farClip);
	float t = ofGetElapsedTimef();
	

	fluidSolver->update();
	
	
	t = t * PI / 180.0 * 10.0;
	lightPos = ofVec3f(250.0 * sin(t), 50.0, 250.0*cos(t));


	depthFbo.begin();
	ofClear(0);
	glClear(0);
	depthPass.begin();
	cam.begin();
	ofEnableDepthTest();
	glEnable(GL_DEPTH_TEST);
	ofMatrix4x4 view = ofGetCurrentViewMatrix();
	ofMatrix4x4 invView = view.getInverse();
	ofMatrix4x4 proj = cam.getProjectionMatrix();
	depthPass.setUniform1f("size", particleSize);
	depthPass.setUniform1f("time", ofGetElapsedTimef());
	depthPass.setUniformMatrix4f("viewMatrix", view);
	depthPass.setUniformMatrix4f("invViewMatrix", view.getInverse());
	depthPass.setUniformMatrix4f("projectionMatrix", proj);
	depthPass.setUniformMatrix4f("invpProjectionMatrix", proj.getInverse());
	fluidSolver->draw();
	depthPass.end();

	//pointLight.setPosition(lightPos);
	//pointLight.draw();
	cam.end();
	depthFbo.end();


	blurFbo1.begin();
	ofEnableDepthTest();
	ofClear(0);
	blurPass.begin();
	blurPass.setUniformTexture("depthTex", depthFbo.getTexture(0), 0);
	blurPass.setUniform2f("direction", ofVec2f(1.0, 0.0));
	blurPass.setUniform1f("blurScale", blurScale);
	blurPass.setUniform1f("blurDepthFallOff", blurDepthFallOff);
	depthFbo.draw(0.0, 0.0);
	blurPass.end();
	blurFbo1.end();


	blurFbo2.begin();
	ofEnableDepthTest();
	ofClear(0);
	blurPass.begin();
	blurPass.setUniformTexture("depthTex", blurFbo1.getTexture(0), 0);
	blurPass.setUniform2f("direction", ofVec2f(0.0, 1.0));
	blurPass.setUniform1f("blurScale", blurScale);
	blurPass.setUniform1f("blurDepthFallOff", blurDepthFallOff);
	blurFbo1.draw(0.0, 0.0);
	blurPass.end();
	blurFbo2.end();


	calcNormalFbo.begin();
	ofEnableDepthTest();
	ofClear(0);
	calcNormalPass.begin();
	calcNormalPass.setUniformTexture("depthTex", blurFbo2.getTexture(0), 0);
	calcNormalPass.setUniformMatrix4f("viewMatrix", view);
	calcNormalPass.setUniformMatrix4f("invProjectionMatrix", proj.getInverse());
	calcNormalPass.setUniform2f("screenScale", ofVec2f(ofGetWidth(), ofGetHeight()));
	blurFbo2.draw(0.0, 0.0);
	calcNormalPass.end();
	calcNormalFbo.end();

	quad.addVertex(ofVec3f(-1.0, -1.0));
	quad.addVertex(ofVec3f(-1.0, 1.0));
	quad.addVertex(ofVec3f(1.0, 1.0));
	quad.addVertex(ofVec3f(1.0, -1.0));

	quad.addTexCoord(ofVec2f(.0, .0));
	quad.addTexCoord(ofVec2f(.0, 1.0));
	quad.addTexCoord(ofVec2f(1.0, 1.0));
	quad.addTexCoord(ofVec2f(1.0, .0));

	renderFbo.begin();
	ofEnableDepthTest();
	ofClear(0);
	renderPass.begin();
	cam.begin();
	renderPass.setUniformTexture("normalTex", calcNormalFbo.getTexture(0), 0);
	renderPass.setUniform3f("lightPos", lightPos);
	renderPass.setUniform3f("camPos", cam.getPosition());
	quad.draw();
	cam.end();
	renderPass.end();
	renderFbo.end();

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0.0, 0.0, 0.0);
	//depthFbo.draw(0.0, 0.0);
	//blurFbo2.draw(0.0, 0.0);

	//calcNormalFbo.draw(0.0, 0.0);
	renderFbo.draw(0.0, 0.0);
	ofDisableDepthTest();
	gui.draw();

	//mrtViewer->preview(calcNormalFbo, depthFbo, blurFbo2);
	mrtViewer->preview(depthFbo, blurFbo2, calcNormalFbo);

	cam.begin();
	//ofDrawAxis(5000);
	cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ' ') {
		depthPass.load("shaders/depth.vert", "shaders/depth.frag", "shaders/depth.geom");
		blurPass.load("shaders/blur.vert", "shaders/blur.frag");
		calcNormalPass.load("shaders/calcNormal.vert", "shaders/calcNormal.frag");
		renderPass.load("shaders/render.vert", "shaders/render.frag");
	}
}
//--------------------------------------------------------------
void ofApp::preLoad() {
	depthPass.setGeometryInputType(GL_POINTS);
	depthPass.setGeometryOutputType(GL_TRIANGLE_STRIP);
	depthPass.setGeometryOutputCount(4);
	depthPass.load("shaders/depth.vert", "shaders/depth.frag", "shaders/depth.geom");
	blurPass.load("shaders/blur.vert", "shaders/blur.frag");
	calcNormalPass.load("shaders/calcNormal.vert", "shaders/calcNormal.frag");
	renderPass.load("shaders/render.vert", "shaders/render.frag");
}
//--------------------------------------------------------------
void ofApp::initGUI() {
	gui.setup();
	gui.add(particleSize.setup("particleSize", 1.7, 0.0, 4.0));
	gui.add(blurScale.setup("blurScale", 0.1, 0.0, 1.0));
	gui.add(blurDepthFallOff.setup("blurDepthFallOff", 3.0, 0.0, 4.0));
	gui.add(nearClip.setup("nearClip", 4.1, 0.1, 10.0));
	gui.add(farClip.setup("farClip", 1500.0, 500.0, 5000.0));
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

	depthFbo.allocate(fboSetting);
	blurFbo1.allocate(fboSetting);
	blurFbo2.allocate(fboSetting);
	calcNormalFbo.allocate(fboSetting);
	renderFbo.allocate(fboSetting);
}