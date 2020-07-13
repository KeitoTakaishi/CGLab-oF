#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	model.loadModel("Model/Winged_Victory.obj", false);
	modelMesh = model.getMesh(0);
	
	gui.setup();
	gui.add(cull.setup("CullingMode", true));
	gui.add(debugMode.setup("DebugMode", false));
	gui.add(nearClip.setup("nearClip", 20.0, 0.1, 500.0));
	gui.add(farClip.setup("farClip", 1000.0, 100.0, 5000.0));
	gui.add(radius.setup("radius", 300, 100.0, 1000.0));
	gui.add(thicknessCoef.setup("thicknessCoef", 30.0, 0.1, 200));
	gui.add(mode.setup("render-mode", 2, 0, 2));
	gui.add(thicknessCol.setup("thickness-col", 
		ofVec3f(1.0, 1.0, 1.), ofVec3f(.0, .0, .0), ofVec3f(1.0, 1.0, 1.0)));

	thichnessShader.load("shaders/pass.vert", "shaders/backRender.frag");
	renderShader.load("shaders/render.vert", "shaders/render.frag");
	initFbo();

	//-----------------------
	for (int i = 0; i < num; i++) {
		ofSpherePrimitive s;
		s.setRadius(ofRandom(15.0 , 100.0));
		float r = 150.0;
		s.setPosition(ofVec3f(ofRandom(-r, r), ofRandom(-r, r), ofRandom(-r, r)));

		//s.setRadius( i * 5.0);
		//float r = 150.0;
		//s.setPosition(ofVec3f(0.0, 0.0, -i * 30.0));
		//color.push_back(ofVec3f(ofRandom(255), ofRandom(255), ofRandom(255)));
		spherees.push_back(s);
	}
	//-----------------------
	

	ofSetFrameRate(300);
	ofSetVerticalSync(false);
	
}

//--------------------------------------------------------------
void ofApp::update(){
	//box.set(farClip);
	ofSetWindowTitle(to_string(ofGetFrameRate()));
	cam.setNearClip(nearClip);
	cam.setFarClip(farClip);
	//box.set(radius);
	//box.set(radius);
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);

	if (!debugMode) {
		//1st pass,‚Æ‚è‚ ‚¦‚¸Fbo‚É”w–Ê‚Ü‚Å‚Ìdepth‚ð‘‚«ž‚Þ
		//glCullFace(GL_FRONT);

		backRenderBuffer.begin();
		ofClear(0);
		//glClearDepth(.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_GREATER);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		cam.begin();
		//box.draw();
		thichnessShader.begin();
		thichnessShader.setUniform1f("geomMode", 0.0);
		thichnessShader.setUniform2f("camPrames", ofVec2f(nearClip, farClip));

		createBox();
		ofPushMatrix();
		ofTranslate(0.0, -150.0, 0.0);
		modelMesh.draw();
		ofPopMatrix();
		cam.end();
		thichnessShader.end();
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		backRenderBuffer.end();



		//2nd pass
		//drawFrontFace(•\–Ê‚ÌƒŒƒ“ƒ_ƒŠƒ“ƒO‚©‚ÂDepthTest‚Í’Êí)

		frontRenderBuffer.begin();
		ofClear(0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearDepth(0.0);
		frontRenderBuffer.clearDepthBuffer(0.0);

		backRenderBuffer.clearDepthBuffer(1.0);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);




		cam.begin();
		//box.draw();
		thichnessShader.begin();
		thichnessShader.setUniform1f("geomMode", 0.0);
		thichnessShader.setUniform2f("camPrames", ofVec2f(nearClip, farClip));
		createBox();
		ofPushMatrix();
		ofTranslate(0.0, -150.0, 0.0);
		modelMesh.draw();
		ofPopMatrix();
		cam.end();
		thichnessShader.end();
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		frontRenderBuffer.end();


		//final pass
		renderBuffer.begin();
		ofClear(0);
		ofBackground(0);
		renderShader.begin();
		renderShader.setUniform1i("mode", mode);
		renderShader.setUniformTexture("backFaceDepthTex", backRenderBuffer.getTexture(0), 0);
		renderShader.setUniformTexture("frontFaceDepthTex", frontRenderBuffer.getTexture(0), 1);
		renderShader.setUniform3f("maxCol", thicknessCol);
		renderShader.setUniform2f("screenScale", ofVec2f(ofGetWidth(), ofGetHeight()));
		renderShader.setUniform2f("camPrames", ofVec2f(nearClip, farClip));
		renderShader.setUniform1f("thicknessCoef", thicknessCoef);

		quad.draw();
		renderShader.end();
		renderBuffer.end();



		//ofDisableDepthTest();
		renderBuffer.draw(0.0, 0.0);

	}
	
	/*
	else {
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		if (cull) {
			//front
			glDepthFunc(GL_LESS);
			glCullFace(GL_BACK);
		}
		else {
			glClearDepth(.0);
			glDepthFunc(GL_GREATER);
			glCullFace(GL_FRONT);
		}
		cam.begin();
		createBox();
		ofPushMatrix();
		ofSetColor(255.0);
		modelMesh.draw(OF_MESH_FILL);
		ofPopMatrix();
		cam.end();

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
	}
	*/
	
	

	ofDrawBitmapString("Fps : " + to_string(ofGetFrameRate()), ofGetWidth() - 200, 30.0);
	if (mode == 0) {
		ofDrawBitmapString("BackFace-Depth", ofGetWidth() - 200, 60.0);
	}else if (mode == 1) {
		ofDrawBitmapString("FrontFace-Depth", ofGetWidth() - 200, 60.0);
	}else if (mode == 2) {
		ofDrawBitmapString("Thickness", ofGetWidth() - 200, 60.0);
	}
	ofDrawBitmapString("DebugMode : " + to_string(debugMode), ofGetWidth() - 200, 90.0);
	gui.draw();
}

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

	backRenderBuffer.allocate(fboSetting);
	frontRenderBuffer.allocate(fboSetting);
	renderBuffer.allocate(fboSetting);


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


void ofApp::keyPressed(int key) {
	if (key == ' ') {
		thichnessShader.load("shaders/pass.vert", "shaders/backRender.frag");
		renderShader.load("shaders/render.vert", "shaders/render.frag");
		initFbo();
	}
}

void ofApp::createBox() {
	float size = radius;
	ofSetRectMode(OF_RECTMODE_CENTER);
	//glEnable(GL_DEPTH_TEST);
	
	
	ofPushMatrix();
	ofSetColor(255.0, 0.0, 0.0);
	ofTranslate(0.0, 0.0, size / 2.0);
	ofDrawRectangle(0.0, 0.0, 0.0, size, size);//front
	ofPopMatrix();

	ofPushMatrix();
	ofSetColor(0.0, 0.0, 255.0);
	ofTranslate(0.0, 0.0, -size / 2.0);
	ofRotateY(180);
	ofDrawRectangle(0.0, 0.0, 0.0, size, size);//back
	ofPopMatrix();

	ofPushMatrix();
	ofRotateY(90);
	ofPushMatrix();
	ofSetColor(255.0, 255.0, 0.0);
	ofTranslate(0.0, 0.0, size / 2.0);
	ofDrawRectangle(0.0, 0.0, 0.0, size, size);//front
	ofPopMatrix();

	ofPushMatrix();
	ofSetColor(0.0, 255.0, 255.0);
	ofTranslate(0.0, 0.0, -size / 2.0);
	ofRotateY(180);
	ofDrawRectangle(0.0, 0.0, 0.0, size, size);//back
	ofPopMatrix();
	ofPopMatrix();

	ofPushMatrix();
	ofRotateX(90);
	ofPushMatrix();
	ofSetColor(255.0, 0.0, 255.0);
	ofTranslate(0.0, 0.0, size / 2.0);
	ofDrawRectangle(0.0, 0.0, 0.0, size, size);//front
	ofPopMatrix();

	ofPushMatrix();
	ofSetColor(0.0, 255.0, 0.0);
	ofTranslate(0.0, 0.0, -size / 2.0);
	ofRotateY(180);
	ofDrawRectangle(0.0, 0.0, 0.0, size, size);//back
	ofPopMatrix();
	ofPopMatrix();

}