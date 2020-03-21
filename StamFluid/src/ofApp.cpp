#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//create bufferes
	swapBuffer = new SwapBuffer(512, 512, GL_RGB32F);
	

	debuggui = new ofxDebugGui();

	advectionShader.load("shaders/pass.vert", "shaders/updateAdvection.frag");
	interactionForceShader.load("shaders/pass.vert", "shaders/interactionForce.frag");
	divergenceShader.load("shaders/pass.vert", "shaders/updateDivergence.frag");
	pressureShader.load("shaders/pass.vert", "shaders/updatePressure.frag");
	velShader.load("shaders/pass.vert", "shaders/updateVelocity.frag");
	resultShader.load("shaders/result");

	tex.load("lena.jpg");
	cout << tex.getTexture().getTextureData().textureID << endl;

	mesh.setMode(OF_PRIMITIVE_POINTS);
	for (int x = 0; x < texRes; x++) {
		for (int y = 0; y < texRes; y++) {
			int i = y * texRes + x;
			//mesh.addVertex(ofVec3f(x / texRes * meshSize.x, y / texRes * meshSize.y, 0.0));
			mesh.addVertex(ofVec3f(x, y, 0.0));
			
			mesh.addTexCoord(ofVec2f(x, y));
		}
	}

	curCursor = ofVec2f(0.0, 0.0);
	preCursor = ofVec2f(0.0, 0.0);

	gui.setup();
	gui.setPosition(20, 20);
	gui.add(area.set("area", 10, 0, 50));
	gui.add(viscosity.set("viscosity", 0.99, 0.9, 1.0));
}

//--------------------------------------------------------------
void ofApp::update(){
	updateAdv();
	interactionForce();
	updateDivergence();
	for (int i = 0; i < 30; i++) {
		updatePressure();
	}
	updateVel();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0.0, 0.0, 0.0);
	//ofDrawBitmapString("1 : velocity", 50, 40);
	//ofDrawBitmapString("2 : debergence", 50, 80);
	//ofDrawBitmapString("3 : pressure", 50, 120);
	ofSetWindowTitle(to_string(ofGetFrameRate()));
	ofEnableDepthTest();

	ofMatrix4x4 viewMatrix;
	ofMatrix4x4 projectionMatrix;
	
	//vel
	{
		debuggui->getShader().begin();

		//swap‚·‚é‚±‚Æ‚ÅsrcÅV‚É‚È‚Á‚Ä‚¢‚é
		debuggui->getShader().setUniformTexture("tex0", swapBuffer->src->getTexture(0), 0);//fbo texture

		ofMatrix4x4 modelMatrix;
		modelMatrix.translate((float)ofGetWidth() / 2.0 - (float)meshSize.x / 2.0 - texRes/2.0, (float)ofGetHeight() / 2.0 - (float)meshSize.y / 2.0, 0.0);
		viewMatrix = ofGetCurrentViewMatrix();
		cam.begin();
		projectionMatrix = cam.getProjectionMatrix();
		cam.end();
		debuggui->getShader().setUniformMatrix4f("model", modelMatrix);
		debuggui->getShader().setUniformMatrix4f("view", viewMatrix);
		debuggui->getShader().setUniformMatrix4f("proj", projectionMatrix);
		mesh.draw();

		debuggui->getShader().end();
	}
	
	
	//dev
	/*
	{
		debuggui->getShader().begin();

		debuggui->getShader().setUniformTexture("tex0", swapBuffer->src->getTexture(1), 1);//fbo texture
		ofMatrix4x4 modelMatrix;
		modelMatrix.translate((float)ofGetWidth() / 2.0 - (float)meshSize.x / 2.0 , (float)ofGetHeight() / 2.0 - (float)meshSize.y / 2.0, 0.0);
		
		viewMatrix = ofGetCurrentViewMatrix();
		cam.begin();
		projectionMatrix = cam.getProjectionMatrix();
		cam.end();
		debuggui->getShader().setUniformMatrix4f("model", modelMatrix);
		debuggui->getShader().setUniformMatrix4f("view", viewMatrix);
		debuggui->getShader().setUniformMatrix4f("proj", projectionMatrix);
		mesh.draw();

		debuggui->getShader().end();
	}
	*/
	//press
	
	{
		//debuggui->getShader().begin();
		resultShader.begin();

		//debuggui->getShader().setUniformTexture("tex0", swapBuffer->src->getTexture(2), 2);//fbo texture
		resultShader.setUniformTexture("tex0", tex.getTexture(), tex.getTexture().getTextureData().textureID);//fbo texture
		resultShader.setUniformTexture("tex1", swapBuffer->src->getTexture(0), 0);//fbo texture
		ofMatrix4x4 modelMatrix;
		modelMatrix.translate((float)ofGetWidth() / 2.0 - (float)meshSize.x / 2.0 + texRes/2.0, (float)ofGetHeight() / 2.0 - (float)meshSize.y / 2.0, 0.0);

		viewMatrix = ofGetCurrentViewMatrix();
		cam.begin();
		projectionMatrix = cam.getProjectionMatrix();
		cam.end();
		resultShader.setUniformMatrix4f("model", modelMatrix);
		resultShader.setUniformMatrix4f("view", viewMatrix);
		resultShader.setUniformMatrix4f("proj", projectionMatrix);
		mesh.draw();

		//debuggui->getShader().end();
		resultShader.end();
	}



	ofDisableDepthTest();
	gui.draw();
	

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ' ') {
		//pass.load("shaders/pass.vert", "shaders/test.frag");
		advectionShader.load("shaders/pass.vert", "shaders/updateAdvection.frag");
		debuggui->load();
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

	//cursor = ofVec2f((float)x/ofGetWidth(), (float)y/ofGetHeight());
	
	
	curCursor = ofVec2f((float)(x), (float)(y));
	if (curCursor.x < 0.0) {
		curCursor.x = 0.0;
	}
	if (curCursor.y < 0.0) {
		curCursor.y = 0.0;
	}
	if (curCursor.x > ofGetWidth()) {
		curCursor.x = ofGetWidth();
	}
	if (curCursor.y > ofGetHeight()) {
		curCursor.y = ofGetHeight();
	}
	mouseVel = curCursor - preCursor;
	preCursor = curCursor;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	isClicked = 1;
	curCursor = ofVec2f((float)(x), (float)(y) );

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	isClicked = 0;
	mouseVel = ofVec2f(0.0, 0.0);
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::updateAdv() {
	swapBuffer->dst->begin();//fbo
	advectionShader.begin();//shader
	swapBuffer->src->activateAllDrawBuffers();

	advectionShader.setUniformTexture("vel", swapBuffer->src->getTexture(0), 0);
	advectionShader.setUniformTexture("div", swapBuffer->src->getTexture(1), 1);
	advectionShader.setUniformTexture("press", swapBuffer->src->getTexture(2), 2);
	
	advectionShader.setUniform1f("dt", dt);
	advectionShader.setUniform2f("cursor", mouseVel);
	advectionShader.setUniform2f("texres", ofVec2f(256.0, 256.0));

	ofClear(0);
	swapBuffer->src->draw(0.0, 0.0);
	

	advectionShader.end();//shader
	swapBuffer->dst->end();//fbo


	swapBuffer->swap();
}
//--------------------------------------------------------------
void ofApp::interactionForce() {
	swapBuffer->dst->begin();//fbo
	interactionForceShader.begin();//shader
	
	swapBuffer->src->activateAllDrawBuffers();
	ofClear(0);
	interactionForceShader.setUniformTexture("vel", swapBuffer->src->getTexture(0), 0);
	interactionForceShader.setUniformTexture("div", swapBuffer->src->getTexture(1), 1);
	interactionForceShader.setUniformTexture("press", swapBuffer->src->getTexture(2), 2);

	ofVec2f cursor = curCursor / ofVec2f(ofGetWidth(), ofGetHeight()) * ofVec2f((float)texRes, (float)texRes);
	cursor.y = texRes - cursor.y;
	interactionForceShader.setUniform2f("cursor", cursor);
	interactionForceShader.setUniform2f("cursorVel", mouseVel);
	interactionForceShader.setUniform1f("state", isClicked);
	interactionForceShader.setUniform1f("area", area);


	
	swapBuffer->src->draw(0.0, 0.0);
	

	interactionForceShader.end();//shader
	swapBuffer->dst->end();//fbo


	swapBuffer->swap();
}
//--------------------------------------------------------------
void ofApp::updateDivergence() {
	swapBuffer->dst->begin();//fbo
	divergenceShader.begin();//shader
	
	swapBuffer->src->activateAllDrawBuffers();
	
	divergenceShader.setUniformTexture("vel", swapBuffer->src->getTexture(0), 0);
	divergenceShader.setUniformTexture("div", swapBuffer->src->getTexture(1), 1);
	divergenceShader.setUniformTexture("press", swapBuffer->src->getTexture(2), 2);
	

	ofClear(0);
	swapBuffer->src->draw(0.0, 0.0);


	divergenceShader.end();//shader
	swapBuffer->dst->end();//fbo


	swapBuffer->swap();
}
//--------------------------------------------------------------
void ofApp::updatePressure() {
	swapBuffer->dst->begin();//fbo
	pressureShader.begin();//shader

	swapBuffer->src->activateAllDrawBuffers();

	pressureShader.setUniformTexture("vel", swapBuffer->src->getTexture(0), 0);
	pressureShader.setUniformTexture("div", swapBuffer->src->getTexture(1), 1);
	pressureShader.setUniformTexture("press", swapBuffer->src->getTexture(2), 2);


	ofClear(0);
	swapBuffer->src->draw(0.0, 0.0);


	pressureShader.end();//shader
	swapBuffer->dst->end();//fbo


	swapBuffer->swap();
}
//--------------------------------------------------------------
void ofApp::updateVel() {
	swapBuffer->dst->begin();//fbo
	velShader.begin();//shader

	swapBuffer->src->activateAllDrawBuffers();

	velShader.setUniformTexture("vel", swapBuffer->src->getTexture(0), 0);
	velShader.setUniformTexture("div", swapBuffer->src->getTexture(1), 1);
	velShader.setUniformTexture("press", swapBuffer->src->getTexture(2), 2);
	velShader.setUniform1f("viscosity", viscosity);



	ofClear(0);
	swapBuffer->src->draw(0.0, 0.0);


	velShader.end();//shader
	swapBuffer->dst->end();//fbo


	swapBuffer->swap();
}