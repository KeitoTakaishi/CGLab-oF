
//render frag‘‚­
//thickness‚É‚Â‚¢‚Ä

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

#ifndef TARGET_PROGRAMMABLE_GL
	
#endif

	ofSetVerticalSync(false);
	//ofSetFrameRate(60);

	ssfr = new SSFR();
	solver = new FluidSolver();

	solver->loadFile();

	ssfr->init();
	ssfr->loadShader();

	cam.setupPerspective(false, 60.0f, 0.1f, 60.0f);
	ofLogNotice("NearClip : " + ofToString(cam.getNearClip()));
	ofLogNotice("FarClip : " + ofToString(cam.getFarClip()));

	
	ofDisableArbTex();
	img[0].load("tex/px.png");
	img[1].load("tex/nx.png");
	img[2].load("tex/py.png");
	img[3].load("tex/ny.png");
	img[4].load("tex/pz.png");
	img[5].load("tex/nz.png");
	cubeMap.setFromImages(256, img[0], img[1], img[2], img[3], img[4], img[5]);
	ofEnableArbTex();
	

	room.set(100);
	room.setResolution(32);
	room.setPosition(0.0, 0.0, 0.0);
	initGUI();

	container.setRadius(10.0);
	container.setResolution(32);
	container.setPosition(0.0, 0.0, 0.0);



	quad.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
	quad.addVertex(ofVec3f(1.0, 1.0, 0.0)); // top-right
	quad.addTexCoord(ofVec2f(1.0f, 1.0f));
	quad.addVertex(ofVec3f(1.0, -1.0, 0.0)); //bottom-right
	quad.addTexCoord(ofVec2f(1.0f, 0.0f));
	quad.addVertex(ofVec3f(-1.0, -1.0, 0.0)); //bottom-left
	quad.addTexCoord(ofVec2f(0.0f, 0.0f));
	quad.addVertex(ofVec3f(-1.0, 1.0, 0.0)); //top-left
	quad.addTexCoord(ofVec2f(0.0f, 1.0f));

	
}

//--------------------------------------------------------------
void ofApp::update(){
	float t =ofGetElapsedTimef();
	float theta = t * DEG_TO_RAD * 10.0f;
	
	cam.setPosition(camRadius * cos(theta), 1.5, camRadius * sin(theta));
	//cam.setPosition(camRadius * cos(theta), -2.0, camRadius * sin(theta));
	cam.lookAt(ofVec3f(0.0, 0.0, 0.0), ofVec3f(0.0, 1.0, 0.0));

}
//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);

	ofMatrix4x4 view;
	ofMatrix4x4 invView;
	ofMatrix4x4 proj;

	//cout << "-----before-----" << endl;
	//cout << view << endl;
	//cout << "----------------" << endl; 

	//Depth Pass
	ssfr->calcDepthBegin();
	ssfr->depthFbo.activateAllDrawBuffers();
	ofEnableDepthTest();
	cam.begin();
	camPosition = cam.getPosition();
	camUp = cam.getUpDir();
	camFov = cam.getFov();
	cam.setNearClip(clips.get().x);
	cam.setFarClip(clips.get().y);
	ssfr->calcDepthPass.begin();


	view = ofGetCurrentViewMatrix();
	invView = view.getInverse();
	proj = cam.getProjectionMatrix();
	//cout << "-----after------" << endl;
	//cout << view << endl;
	//cout << "----------------" << endl;
	
	ssfr->calcDepthPass.setUniformMatrix4f("v", view);
	ssfr->calcDepthPass.setUniformMatrix4f("p", proj);
	ssfr->calcDepthPass.setUniformMatrix4f("iv", invView);
	ssfr->calcDepthPass.setUniformMatrix4f("ip", proj.getInverse());
	ssfr->calcDepthPass.setUniform1f("size", size);
	ssfr->calcDepthPass.setUniform1f("time", ofGetElapsedTimef());
	solver->draw();
	ssfr->calcDepthPass.end();

	//ofSetColor(255.0);
	//box.drawWireframe();
	cam.end();
	ssfr->calcDepthEnd();


	//1st BlurPass
	ssfr->blur1Fbo.begin();
	ssfr->blurPass.begin();
	ssfr->blurPass.setUniformTexture("depthTex", ssfr->depthFbo.getTexture(0), 0);
	ssfr->blurPass.setUniform2f("direction", ofVec2f(1.0, 0.0));
	ssfr->blurPass.setUniform1f("blurScale", blurScale);
	ssfr->blurPass.setUniform1f("blurDepthFallOff", blurDepthFallOff);
	ssfr->depthFbo.draw(0.0, 0.0);
	ssfr->blurPass.end();
	ssfr->blur1Fbo.end();


	//2st Blur Pass
	ssfr->blur2Fbo.begin();
	ssfr->blurPass.begin();
	ssfr->blurPass.setUniformTexture("depthTex", ssfr->blur1Fbo.getTexture(0), 1);
	ssfr->blurPass.setUniform2f("direction", ofVec2f(0.0, 1.0));
	ssfr->blurPass.setUniform1f("blurScale", blurScale);
	ssfr->blurPass.setUniform1f("blurDepthFallOff", blurDepthFallOff);
	ssfr->blur1Fbo.draw(0.0, 0.0);
	ssfr->blurPass.end();
	ssfr->blur2Fbo.end();


	//Normal
	ssfr->normalFbo.begin();
	ssfr->calcNormalPass.begin();
	ssfr->calcNormalPass.setUniformTexture("depthTex", ssfr->blur2Fbo.getTexture(0), 2);
	ssfr->calcNormalPass.setUniformMatrix4f("viewMatrix", view);
	ssfr->calcNormalPass.setUniformMatrix4f("invProjectionMatrix", proj.getInverse());
	ssfr->calcNormalPass.setUniform2f("screenScale", ofVec2f(ofGetWidth(), ofGetHeight()));
	ssfr->calcNormalPass.setUniform2f("camClips", ofVec2f(clips.get().x, clips.get().y));
	ssfr->blur1Fbo.draw(0.0, 0.0);
	ssfr->calcNormalPass.end();
	ssfr->normalFbo.end();

	
	//Thickness
	ssfr->thicknessFbo.begin();
	ofClear(0);
	ofDisableDepthTest();
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	cam.begin();
	cam.setNearClip(clips.get().x);
	cam.setFarClip(clips.get().y);
	ssfr->calcThicknessPass.begin();
	ssfr->calcThicknessPass.setUniformMatrix4f("v", view);
	ssfr->calcThicknessPass.setUniformMatrix4f("p", proj);
	ssfr->calcThicknessPass.setUniformMatrix4f("iv", invView);
	ssfr->calcThicknessPass.setUniformMatrix4f("ip", proj.getInverse());
	ssfr->calcThicknessPass.setUniform1f("size", size);
	solver->draw();
	ssfr->calcThicknessPass.end();
	cam.end();
	ssfr->thicknessFbo.end();

		
	//render
	//Sphere
	
	ofEnableDepthTest();
	
	ssfr->renderFbo.begin();
	ofClear(0, 0.0, 0.0);
	cubeMap.bind();
	cam.begin();
	ssfr->cubeMapPass.begin();
	ssfr->cubeMapPass.setUniform1i("reflection", 1);
	ssfr->cubeMapPass.setUniform3f("cameraPos", cam.getPosition());
	room.draw();
	ssfr->cubeMapPass.end();
	cam.end();
	cubeMap.unbind();
	ssfr->renderFbo.end();
	

	//RayMarching
	ofEnableAlphaBlending();

	//Fluid
	ssfr->renderFbo.begin();
	//ofClear(0);//Debug
	cubeMap.bind();
	ssfr->renderSSFRPass.begin();
	ssfr->renderSSFRPass.setUniformTexture("depthTex", ssfr->depthFbo.getTexture(0), 3);
	ssfr->renderSSFRPass.setUniformTexture("positionTex", ssfr->depthFbo.getTexture(1), 4);
	ssfr->renderSSFRPass.setUniformTexture("normalTex", ssfr->normalFbo.getTexture(0), 5);
	ssfr->renderSSFRPass.setUniformTexture("thicknessTex", ssfr->thicknessFbo.getTexture(0), 6);
	//ofMatrix4x4 _v = getViewMatrix(cam.getPosition(), camUp, ofVec3f(0.0, 0.0, 0.0));
	ssfr->renderSSFRPass.setUniformMatrix4f("viewMatrix", view);
	ssfr->renderSSFRPass.setUniformMatrix4f("invViewMatrix", invView);
	ssfr->renderSSFRPass.setUniform3f("lightPos", lightPos);
	ssfr->renderSSFRPass.setUniform3f("camPos", cam.getGlobalPosition());
	ssfr->renderSSFRPass.setUniform1f("shininess", shininess);
	ssfr->renderSSFRPass.setUniform3f("absorbK", absorbK);
	ssfr->thicknessFbo.draw(0.0, 0.0);//quad‚É•Ï‚¦‚½‚Ù‚¤‚ª‚æ‚³‚»‚¤
	ssfr->renderSSFRPass.end();
	cubeMap.unbind();
	ssfr->renderFbo.end();

	
	//Implicit Surface
	ssfr->renderFbo.begin();
	cubeMap.bind();
	ssfr->rayMarchingPass.begin();
	ssfr->rayMarchingPass.setUniformMatrix4f("view", view);
	ssfr->rayMarchingPass.setUniform2f("resolution", ofVec2f(ofGetWidth(), ofGetHeight()));
	ssfr->rayMarchingPass.setUniform3f("camUp", camUp);
	ssfr->rayMarchingPass.setUniform3f("camPosition", camPosition);
	ssfr->rayMarchingPass.setUniform3f("lightPosition", lightPos);
	ssfr->rayMarchingPass.setUniform1f("fov", camFov);
	ssfr->thicknessFbo.draw(0.0, 0.0);//quad‚É•Ï‚¦‚½‚Ù‚¤‚ª‚æ‚³‚»‚¤
	ssfr->rayMarchingPass.end();
	cubeMap.unbind();
	ssfr->renderFbo.end();



	//2D
	ofDisableDepthTest();
	//ssfr->depthFbo.draw(0.0, 0.0);
	//ssfr->blur1Fbo.draw(0.0, 0.0);
	//ssfr->blur2Fbo.draw(0.0, 0.0);
	//ssfr->normalFbo.draw(0.0, 0.0);
	//ssfr->thicknessFbo.draw(0.0, 0.0);
	ssfr->renderFbo.draw(0.0, 0.0);
	
	
	ofPushStyle();
	ofSetColor(125, 100.0);
	ofDrawRectangle(ofGetWidth() - 200, 0.0, 200, 80);
	ofSetColor(255.0, 255.0);
	ofDrawBitmapString("particle num : " + ofToString(solver->particle.getVertices().size()), ofGetWidth() - 180, 20.0);
	ofDrawBitmapString("fps : " + ofToString(ofGetFrameRate()), ofGetWidth() - 180, 40.0);
	ofPopStyle();
	


	depthGui.draw();
	blurGui.draw();
	renderGui.draw();

	if (ofGetFrameNum() == 10000) {
		clock_t end = clock();
		const double time = static_cast<double>(end) / CLOCKS_PER_SEC * 1000.0;

		cout << time << "ms" << endl;

	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key = ' ') {
		ssfr->loadShader();
	}
}

//--------------------------------------------------------------
void ofApp::initGUI() {
	depthGui.setup();
	depthGui.setPosition(0.0, 0.0);
	depthGui.setName("CalcDepth");
	depthGui.add(camRadius.set("Cam Radius", 4.0, 0.1, 10.0));
	depthGui.add(size.set("Particle Size", 0.04, 0.01, 0.1));
	depthGui.add(clips.set("Near-Far", ofVec2f(0.3f, 100.0f), ofVec2f(0.1f, 1.0f), ofVec2f(10.0f, 1000.0f)));


	blurGui.setup();
	blurGui.setPosition(0.0, 150.0);
	blurGui.setName("Blur");
	blurGui.add(blurScale.set("BlurScale", 0.09, 0.0, 1.0));
	blurGui.add(blurDepthFallOff.set("BlurDepthFallOff", 2.0, 0.0, 30.0));


	renderGui.setup();
	renderGui.setPosition(0.0, 300.0);
	renderGui.setName("Render(SSFR)");
	renderGui.add(lightPos.setup("LightPos", ofVec3f(12.0, -100.0, 12.0), ofVec3f(-300.0, -300.0, -300.0), ofVec3f(300.0, 300.0, 300.0)));
	renderGui.add(shininess.setup("Shininess", 100.0, 0.0, 300.0));
	renderGui.add(absorbK.setup("AbsorbK", ofVec3f(0.95, 0.15, 0.2), ofVec3f(.0, .0, .0), ofVec3f(1.0, 1.0, 1.0)));
}
//--------------------------------------------------------------
ofMatrix4x4 ofApp::getViewMatrix(ofVec3f eye, ofVec3f up, ofVec3f center) {
	float eyeX = eye[0], eyeY = eye[1], eyeZ = eye[2],
		upX = up[0], upY = up[1], upZ = up[2],
		centerX = center[0], centerY = center[1], centerZ = center[2];

	//if (eyeX == centerX && eyeY == centerY && eyeZ == centerZ) { return this.identity(dest); }
	float x0, x1, x2, y0, y1, y2, z0, z1, z2, l;
	z0 = eyeX - center[0]; z1 = eyeY - center[1]; z2 = eyeZ - center[2];
	l = 1 / sqrt(z0 * z0 + z1 * z1 + z2 * z2);
	z0 *= l; z1 *= l; z2 *= l;
	x0 = upY * z2 - upZ * z1;
	x1 = upZ * z0 - upX * z2;
	x2 = upX * z1 - upY * z0;
	l = sqrt(x0 * x0 + x1 * x1 + x2 * x2);
	if (!l) {
		x0 = 0; x1 = 0; x2 = 0;
	}
	else {
		l = 1 / l;
		x0 *= l; x1 *= l; x2 *= l;
	}
	y0 = z1 * x2 - z2 * x1; y1 = z2 * x0 - z0 * x2; y2 = z0 * x1 - z1 * x0;
	l = sqrt(y0 * y0 + y1 * y1 + y2 * y2);
	if (!l) {
		y0 = 0; y1 = 0; y2 = 0;
	}
	else {
		l = 1 / l;
		y0 *= l; y1 *= l; y2 *= l;
	}
	ofMatrix4x4 view = ofMatrix4x4(
		x0, y0, z0, 0.0,
		x1, y1, z1, 0.0,
		x2, y2, z2, 0.0,
		-(x0 * eyeX + x1 * eyeY + x2 * eyeZ), -(y0 * eyeX + y1 * eyeY + y2 * eyeZ), -(z0 * eyeX + z1 * eyeY + z2 * eyeZ), 1.0
	);

	return view;
}

//--------------------------------------------------------------
ofMatrix4x4 ofApp::getProjMatrix(float fov, float aspect, float n, float f) {
	float r = 1 / tan(fov * PI / 360);
	float _far = f;
	float _near = n;
	float d = _far - _near;
	ofMatrix4x4 proj = ofMatrix4x4(
		r / aspect, 0.0, 0.0, 0.0,
		0.0, r, 0.0, 0.0,
		0.0, 0.0, -(_far + _near) / d, -1.0,
		0.0, 0.0, -(_far * _near * 2) / d, 0.0
	);
	return proj;
}