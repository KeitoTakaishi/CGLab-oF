#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	createFrameBuffer();
	deferredShader.load("Shaders/deferred");
	renderPath.load("Shaders/render");


	float w = 1.0;
	float h = 1.0;
	quad.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
	quad.addVertex(ofVec3f(w, h, 0.0));
	quad.addTexCoord(ofVec2f(1.0f, 1.0f));
	quad.addVertex(ofVec3f(w, -h, 0.0));
	quad.addTexCoord(ofVec2f(1.0f, 0.0f));
	quad.addVertex(ofVec3f(-w, -h, 0.0));
	quad.addTexCoord(ofVec2f(0.0f, 0.0f));
	quad.addVertex(ofVec3f(-w, h, 0.0));
	quad.addTexCoord(ofVec2f(0.0f, 1.0f));


	for (int i = 0; i < 100; i++) {
		pos[i] = ofVec3f(ofRandom(-500, 500), ofRandom(-500, 500), ofRandom(-500, 500));
	}

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	glEnable(GL_DEPTH_TEST);
	
	//----------------------------------------
	//FirstPath-------------------------------
	//----------------------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	deferredShader.begin();

	cam.begin();
	ofMatrix4x4 proj, view;
	view = ofGetCurrentViewMatrix();
	proj = cam.getProjectionMatrix();
	cam.end();


	for (int i = 0; i < 100; i++) {
		ofMatrix4x4 model;
		model.translate(pos[i]);
		deferredShader.setUniformMatrix4f("model", model);
		ofSpherePrimitive s = ofSpherePrimitive(10, 8);
		s.draw();
	}

	ofPlanePrimitive plane = ofPlanePrimitive(100, 100, 10, 10);
	plane.draw();
	

	deferredShader.setUniformMatrix4f("view", view);
	deferredShader.setUniformMatrix4f("proj", proj);
	//geometryPass.setUniformTexture("tex0", albedoTexture, albedoTexture.getTextureData().textureID);

	deferredShader.end();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//----------------------------------------
	//SecondPath
	//----------------------------------------
	
	renderPath.begin();
	renderPath.setUniformTexture("gPosition", GL_TEXTURE_2D, gPosition, 0);
	renderPath.setUniformTexture("gNormal", GL_TEXTURE_2D, gNormal, 1);
	quad.draw(OF_MESH_FILL);
	renderPath.end();
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ' ') {
		deferredShader.load("Shaders/deferred");
		renderPath.load("Shaders/render");
	}
}

//--------------------------------------------------------------
void ofApp::createFrameBuffer() {
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	ofVec2f res = ofVec2f(ofGetWidth(), ofGetHeight());

	//position texture
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, res.x, res.y,
		0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

	//normal texture
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, res.x, res.y,
		0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

	//gAlbedo
	glGenTextures(1, &gAlbedo);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, res.x, res.y,
		0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedo, 0);


	GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);


	//depth
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, res.x, res.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}