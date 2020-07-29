#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	shadowMap = new ShadowMap();
	mainCam.lookAt(ofVec3f(0.0, 0.0, 0.0), ofVec3f(0.0, 1.0, 0.0));
	mainCam.setPosition(1000, 1000, 0.0);

	
	mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
	float w = 1.0;
	float h = 1.0;
	float iw = 1.0;
	float ih = 1.0;

	mesh.addVertex(ofVec3f(w, h, 0.0));
	mesh.addTexCoord(ofVec2f(iw, ih));

	mesh.addVertex(ofVec3f(w, -h, 0.0));
	mesh.addTexCoord(ofVec2f(iw, 0.0f));

	mesh.addVertex(ofVec3f(-w, -h, 0.0));
	mesh.addTexCoord(ofVec2f(0.0f, 0.0f));

	mesh.addVertex(ofVec3f(-w, h, 0.0));
	mesh.addTexCoord(ofVec2f(0.0f, ih));

	previewShader.load("shaders/viewport.vert", "shaders/depthPreview.frag");

	shadowMap->initFbo();
	shadowMap->setVirtualCam();


	//cout << shadowMap->fbo.getDepthTexture().getTextureData().glInternalFormat << endl;
	
}

//--------------------------------------------------------------
void ofApp::update(){

	float time = ofGetElapsedTimef() * 0.1;
	shadowMap->setPosition(ofVec3f(500 * cos(time * 2), 1500, 500 * sin(time * 2)));
	shadowMap->lookAt(ofVec3f(0.0, 0.0, 0.0));
	
	

}

//--------------------------------------------------------------
void ofApp::draw(){
	
	/*
	previewShader.begin();
	previewShader.setUniformTexture("depth_tex", shadowMap->fbo.getDepthTexture(), 1);
	mesh.draw();
	previewShader.end();
	*/

	ofEnableDepthTest();
	shadowMap->debugLight();
	shadowMap->beginDepth();
	renderScene();
	shadowMap->endDepth();

	
	shadowMap->begin(mainCam);
	renderScene();
	shadowMap->end(mainCam);


	
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	previewShader.load("shaders/viewport.vert", "shaders/depthPreview.frag");
}

//--------------------------------------------------------------
void ofApp::renderScene() {
	ofEnableDepthTest();
	//light


	float time = ofGetElapsedTimef();

	// Cone
	ofPushMatrix();
	ofTranslate(-500, 300, -500);
	ofRotate(time * 30.0, 1.0, 0.7, 0.4);
	ofDrawCone(200, 300);
	ofPopMatrix();

	// Box
	ofPushMatrix();
	ofTranslate(0, 300, -500);
	ofRotate(time * 30.0, 1.0, 0.7, 0.4);
	ofDrawBox(200, 200, 200);
	ofPopMatrix();

	// IcoSphere
	ofPushMatrix();
	ofTranslate(500, 300, -500);
	ofRotate(time * 30.0, 1.0, 0.7, 0.4);
	ofDrawIcoSphere(200);
	ofPopMatrix();

	// Cylinder
	ofPushMatrix();
	ofTranslate(-500, 300, 500);
	ofRotate(time * 30.0, 1.0, 0.7, 0.4);
	ofDrawCylinder(200, 300);
	ofPopMatrix();

	// Plane
	ofPushMatrix();
	ofTranslate(0, 300, 500);
	ofRotate(time * 30.0, 1.0, 0.7, 0.4);
	ofDrawPlane(300, 300);
	ofPopMatrix();

	// Sphere
	ofPushMatrix();
	ofTranslate(500, 300, 500);
	ofRotate(time * 30.0, 1.0, 0.7, 0.4);
	ofDrawSphere(200);
	ofPopMatrix();

	// Ground
	ofDrawBox(0, -5, 0, 5000, 10, 5000);

	glDisable(GL_DEPTH_TEST);
}