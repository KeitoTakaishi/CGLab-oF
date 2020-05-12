#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	depthPass.setGeometryInputType(GL_POINTS);
	depthPass.setGeometryOutputType(GL_TRIANGLE_STRIP);
	depthPass.setGeometryOutputCount(4);

	//shader.setGeometryOutputType(GL_POINTS);
	//shader.setGeometryOutputCount(1);

	depthPass.load("shaders/depth.vert", "shaders/depth.frag", "shaders/depth.geom");
	blurPass.load("shaders/blur.vert", "shaders/blur.frag");
	calcNormalPass.load("shaders/calcNormal.vert", "shaders/calcNormal.frag");


	vbo.setMode(OF_PRIMITIVE_POINTS);
	particleNum = 5000;
	for (int i = 0; i < particleNum; i++) {
		ofVec3f pos = ofVec3f(ofRandom(-300, 300), ofRandom(-300, 300), ofRandom(-300, 300));
		vertices.push_back(pos);
		vbo.addVertex(pos);
	}
	
	cam.setupPerspective(true, 60.0f, 0.01f, 500.0f);




	ofFbo::Settings fboSetting;
	fboSetting.width = ofGetWidth();
	fboSetting.height = ofGetHeight();
	fboSetting.numColorbuffers = 1;
	fboSetting.useDepth = true;
	fboSetting.internalformat = GL_RGBA32F;
	fboSetting.wrapModeHorizontal = GL_CLAMP_TO_EDGE;
	fboSetting.wrapModeVertical = GL_CLAMP_TO_EDGE;
	fboSetting.minFilter = GL_LINEAR;
	fboSetting.maxFilter = GL_LINEAR;

	//depthFbo.allocate(ofGetWidth(), ofGetHeight());
	//blurFbo1.allocate(ofGetWidth(), ofGetHeight());
	//blurFbo2.allocate(ofGetWidth(), ofGetHeight());
	//calcNormalFbo.allocate(ofGetWidth(), ofGetHeight());

	depthFbo.allocate(fboSetting);
	blurFbo1.allocate(fboSetting);
	blurFbo2.allocate(fboSetting);
	calcNormalFbo.allocate(fboSetting);



	gui.setup();
	gui.add(blurScale.setup("blurScale", 0.1, 0.0, 1.0));
	gui.add(blurDepthFallOff.setup("blurDepthFallOff", 0.4, 0.0, 1.0));
}

//--------------------------------------------------------------
void ofApp::update(){

	float t = ofGetElapsedTimef();
	for (int i = 0; i < particleNum; i++) {
		vertices[i] -= ofVec3f(0.0, 1.0, 0.0);
		if (vertices[i].y < -300.0) vertices[i].y = 300.0;
		vbo.setVertex(i, vertices[i]);
	}

	depthFbo.begin();
	ofEnableDepthTest();
	ofClear(0);
	depthPass.begin();
	cam.begin();
	ofMatrix4x4 view = ofGetCurrentViewMatrix();
	ofMatrix4x4 invView = view.getInverse();
	ofMatrix4x4 proj = cam.getProjectionMatrix();
	depthPass.setUniform1f("size", 7.0f);
	depthPass.setUniform1f("time", ofGetElapsedTimef());
	depthPass.setUniformMatrix4f("viewMatrix", view);
	depthPass.setUniformMatrix4f("invViewMatrix", view.getInverse());
	depthPass.setUniformMatrix4f("projectionMatrix", proj);
	depthPass.setUniformMatrix4f("invpProjectionMatrix", proj.getInverse());
	vbo.draw(OF_MESH_FILL);
	cam.end();
	depthPass.end();
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
	calcNormalPass.setUniformMatrix4f("invProjectionMatrix", proj.getInverse());
	calcNormalPass.setUniformMatrix4f("viewMatrix", view);
	calcNormalPass.setUniform2f("screenScale", ofVec2f(ofGetWidth(), ofGetHeight()));
	blurFbo2.draw(0.0, 0.0);
	calcNormalPass.end();
	calcNormalFbo.end();
}

//--------------------------------------------------------------
void ofApp::draw(){
	//depthFbo.draw(0.0, 0.0);
	//blurFbo2.draw(0.0, 0.0);
	calcNormalFbo.draw(0.0, 0.0);
	ofDisableDepthTest();
	gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ' ') {
		depthPass.load("shaders/depth.vert", "shaders/depth.frag", "shaders/depth.geom");
		blurPass.load("shaders/blur.vert", "shaders/blur.frag");
		calcNormalPass.load("shaders/calcNormal.vert", "shaders/calcNormal.frag");
	}
}
