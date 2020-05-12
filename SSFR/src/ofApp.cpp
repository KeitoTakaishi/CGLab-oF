#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	shader.setGeometryInputType(GL_POINTS);
	shader.setGeometryOutputType(GL_TRIANGLE_STRIP);
	shader.setGeometryOutputCount(4);

	//shader.setGeometryOutputType(GL_POINTS);
	//shader.setGeometryOutputCount(1);

	shader.load("shaders/depth.vert", "shaders/depth.frag", "shaders/depth.geom");
	//shader.load("shaders/depth.vert", "shaders/depth.frag");


	vbo.setMode(OF_PRIMITIVE_POINTS);
	particleNum = 5000;
	for (int i = 0; i < particleNum; i++) {
		ofVec3f pos = ofVec3f(ofRandom(-300, 300), ofRandom(-300, 300), ofRandom(-300, 300));
		vbo.addVertex(pos);
	}
	cam.setupPerspective(true, 60.0f, 0.01f, 500.0f);

	depthPass.allocate(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::update(){
	depthPass.begin();
	ofEnableDepthTest();
	ofClear(0);
	shader.begin();
	cam.begin();
	ofMatrix4x4 view = ofGetCurrentViewMatrix();
	ofMatrix4x4 invView = view.getInverse();
	ofMatrix4x4 proj = cam.getProjectionMatrix();
	shader.setUniform1f("size", 7.0f);
	shader.setUniformMatrix4f("viewMatrix", view);
	shader.setUniformMatrix4f("invViewMatrix", view.getInverse());
	shader.setUniformMatrix4f("projectionMatrix", proj);
	shader.setUniformMatrix4f("invpProjectionMatrix", proj.getInverse());
	vbo.draw(OF_MESH_FILL);
	cam.end();
	shader.end();
	depthPass.end();

}

//--------------------------------------------------------------
void ofApp::draw(){
	depthPass.draw(0.0, 0.0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ' ') {
		shader.load("shaders/depth.vert", "shaders/depth.frag", "shaders/depth.geom");

	}
}
