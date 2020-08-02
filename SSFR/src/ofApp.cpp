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
	mrtViewer = new ofxMRTViewer(3);//depth, blur, normal, thickness

	
	cam.setupPerspective(true, 60.0f, nearClip, farClip);


	ofDisableArbTex();
	img[0].load("tex/cube_PX.png");
	img[1].load("tex/cube_NX.png");
	img[2].load("tex/cube_PY.png");
	img[3].load("tex/cube_NY.png");
	img[4].load("tex/cube_PZ.png");
	img[5].load("tex/cube_NZ.png");
	cubeMap.setFromImages(256, img[0], img[1], img[2], img[3], img[4], img[5]);
	ofEnableArbTex();
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


	//--------------------------------------------------------
	//depth
	
		depthFbo.begin();
		ofClear(0);
		depthPass.begin();
		cam.begin();
		ofEnableDepthTest();

		ofMatrix4x4 view = ofGetCurrentViewMatrix();
		ofMatrix4x4 invView = view.getInverse();
		ofMatrix4x4 proj = cam.getProjectionMatrix();
		depthPass.setUniform1f("size", particleSize);
		depthPass.setUniform1f("time", ofGetElapsedTimef());
		depthPass.setUniform2f("camClips", ofVec2f(nearClip, farClip));
		depthPass.setUniformMatrix4f("viewMatrix", view);
		depthPass.setUniformMatrix4f("invViewMatrix", view.getInverse());
		depthPass.setUniformMatrix4f("projectionMatrix", proj);
		depthPass.setUniformMatrix4f("invpProjectionMatrix", proj.getInverse());
		fluidSolver->draw();
		depthPass.end();

		cam.end();
		depthFbo.end();
	
	//--------------------------------------------------------
	//blur1
	{
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
	}
	//--------------------------------------------------------
	//blur2
	{
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
	}
	//--------------------------------------------------------
	//normal
	{
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
	}
	//--------------------------------------------------------
	//thickness
	{
		ofDisableDepthTest();
		ofEnableBlendMode(OF_BLENDMODE_ADD);
		thicknessFbo.begin();
		ofClear(0);
		thicknessPass.begin();
		cam.begin();
		thicknessPass.setUniform1f("size", particleSize);
		//thicknessPass.setUniform1f("time", ofGetElapsedTimef());
		thicknessPass.setUniformMatrix4f("viewMatrix", view);
		thicknessPass.setUniformMatrix4f("invViewMatrix", view.getInverse());
		thicknessPass.setUniformMatrix4f("projectionMatrix", proj);
		thicknessPass.setUniformMatrix4f("invpProjectionMatrix", proj.getInverse());
		thicknessPass.setUniform2f("camClips", ofVec2f(nearClip, farClip));
		fluidSolver->draw();
		thicknessPass.end();
		cam.end();
		thicknessFbo.end();
		ofDisableBlendMode();
	}
	//--------------------------------------------------------
	//render
	//cubeMap.bind();
	renderFbo.begin();
	ofEnableDepthTest();
	ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofClear(0);
	renderPass.begin();
	cam.begin();
	renderPass.setUniformTexture("normalTex", calcNormalFbo.getTexture(0), 0);
	renderPass.setUniformTexture("thicknessTex", thicknessFbo.getTexture(0), 1);
	renderPass.setUniform3f("lightPos", lightPos);
	renderPass.setUniform3f("camPos", cam.getPosition());
	renderPass.setUniform1f("alphaCoef", alphaCoef);
	
	renderPass.setUniform3f("_albedoColor", albedoColor);
	renderPass.setUniform3f("_lightDir", lightDir);
	renderPass.setUniform3f("_lightCoef", lightCoef);
	renderPass.setUniform3f("_absorbK", absorbK);
	renderPass.setUniform1i("_renderMode", renderMode);
	
	quad.draw();
	cam.end();
	renderPass.end();
	renderFbo.end();
	ofDisableBlendMode();
	//cubeMap.unbind();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0.0, 0.0, 0.0);

	ofEnableAlphaBlending();
	//thicknessFbo.draw(0.0, 0.0);
	renderFbo.draw(0.0, 0.0);
	ofDisableDepthTest();
	gui.draw();
	//mrtViewer->preview(depthFbo, blurFbo2, calcNormalFbo);
	if (isPreview) {
		//mrtViewer->preview(depthFbo, thicknessFbo, calcNormalFbo);
	}
	cam.begin();
	//ofDrawAxis(5000);
	cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ' ') {
		preLoad();
	}
	else if ('p') {
		isPreview != isPreview;
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

	thicknessPass.setGeometryInputType(GL_POINTS);
	thicknessPass.setGeometryOutputType(GL_TRIANGLE_STRIP);
	thicknessPass.setGeometryOutputCount(4);
	thicknessPass.load("shaders/thickness.vert", "shaders/thickness.frag", "shaders/thickness.geom");

	renderPass.load("shaders/render.vert", "shaders/render.frag");
}
//--------------------------------------------------------------
void ofApp::initGUI() {
	gui.setup();
	gui.add(particleSize.setup("particleSize", 6.4, 0.0, 20.0));
	gui.add(blurScale.setup("blurScale", 0.08, 0.0, 1.0));
	gui.add(blurDepthFallOff.setup("blurDepthFallOff", 10.0, 0.0, 30.0));
	gui.add(nearClip.setup("nearClip", 0.4, 0.1, 10.0));
	gui.add(farClip.setup("farClip", 1500.0, 20.0, 5000.0));
	gui.add(alphaCoef.setup("alphaCoef", 0.1, 0.0, 1.0));
	gui.add(albedoColor.setup("albedoColor", ofVec3f(0.0, 0.8, 1.0), ofVec3f(0.0, 0.0, 0.0), ofVec3f(1.0, 1.0, 1.0)));
	gui.add(lightDir.setup("lightDir", ofVec3f(0.0, 0.8, 1.0), ofVec3f(-1.0, -1.0, -1.0), ofVec3f(1.0, 1.0, 1.0)));
	gui.add(lightCoef.setup("_lightCoef", ofVec3f(0.1, 0.1, 0.2), ofVec3f(.0, .0, .0), ofVec3f(1.0, 1.0, 1.0)));
	//gui.add(absorbK.setup("absorbK", ofVec3f(0.15, 0.091, 0.02), ofVec3f(.0, .0, .0), ofVec3f(1.0, 1.0, 1.0)));
	gui.add(absorbK.setup("absorbK", ofVec3f(0.34, 0.35, 0.34), ofVec3f(.0, .0, .0), ofVec3f(1.0, 1.0, 1.0)));
	gui.add(renderMode.setup("renderMode", 0, 0, 2));
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
	thicknessFbo.allocate(fboSetting);
	renderFbo.allocate(fboSetting);


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