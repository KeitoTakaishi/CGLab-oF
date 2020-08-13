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
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	preLoad();
	initFbo();
	initGBuffer();
	initGUI();

	fluidSolver = new FluidSolver();
	fluidSolver->init();
	mrtViewer = new ofxMRTViewer(4);//depth, blur, normal, thickness

	
	cam.setupPerspective(true, 60.0f, nearClip, farClip);


	//CubeMap
	ofDisableArbTex();
	img[0].load("tex/cube_PX.png");
	img[1].load("tex/cube_NX.png");
	img[2].load("tex/cube_PY.png");
	img[3].load("tex/cube_NY.png");
	img[4].load("tex/cube_PZ.png");
	img[5].load("tex/cube_NZ.png");

	
	cubeMap.setFromImages(256, img[0], img[1], img[2], img[3], img[4], img[5]);
	ofEnableArbTex();

	//initLight
	pointLight.set(30.0, 36);
	cam.setPosition(0.0, 0.0, 50.0);
}
//--------------------------------------------------------------
void ofApp::update(){
	ofSetWindowTitle(to_string(ofGetFrameRate()));
	cam.setNearClip(nearClip);
	cam.setFarClip(farClip);
	float t = ofGetElapsedTimef();
	//fluidSolver->update(cam);
	t = t * PI / 180.0 * 10.0;
	//lightPos = ofVec3f(250.0 * sin(t), 50.0, 250.0*cos(t));


	ofMatrix4x4 view = ofGetCurrentViewMatrix();
	ofMatrix4x4 invView = view.getInverse();
	ofMatrix4x4 proj = cam.getProjectionMatrix();
	//--------------------------------------------------------
	//Gbuffer-RayMarchig
	g_buffer.begin();
	
	//g_buffer.activateAllDrawBuffers();
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ofClear(0);
	raymarchPass.begin();
	raymarchPass.setUniform2f("u_resolution", ofVec2f(ofGetWidth(), ofGetHeight()));
	raymarchPass.setUniform3f("u_camPos", cam.getGlobalPosition());
	raymarchPass.setUniform3f("u_camUp", cam.getUpDir());
	raymarchPass.setUniform1f("u_fov", cam.getFov());
	raymarchPass.setUniform1f("u_far", cam.getFarClip());
	raymarchPass.setUniform1f("u_near", cam.getNearClip());
	raymarchPass.setUniformMatrix4f("view", view);
	raymarchPass.setUniformMatrix4f("projection", proj);
	raymarchPass.setUniform1f("u_radius", 250.0);
	quad_raymarch.draw();
	raymarchPass.end();
	
	g_buffer.end();

	//--------------------------------------------------------
	//depth
	depthFbo.begin();
	depthPass.begin();
	depthFbo.activateAllDrawBuffers();
	ofClear(0);
	cam.begin();
	ofEnableDepthTest();
	proj = cam.getProjectionMatrix();
		
	depthPass.setUniform1f("size", particleSize);
	//depthPass.setUniform1f("time", ofGetElapsedTimef());
	depthPass.setUniform2f("camClips", ofVec2f(nearClip, farClip));
	depthPass.setUniformMatrix4f("viewMatrix", view);
	depthPass.setUniformMatrix4f("invViewMatrix", view.getInverse());
	depthPass.setUniformMatrix4f("projectionMatrix", proj);
	depthPass.setUniformMatrix4f("invProjectionMatrix", proj.getInverse());
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
		calcNormalPass.setUniform2f("camClips", ofVec2f(nearClip, farClip));
		//GLuint prog = calcNormalPass.getProgram();
		calcNormalPass.setUniformTexture("depthTex", blurFbo2.getTexture(0), 0);
		view = ofGetCurrentViewMatrix();
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

	//Fluid-Render
	
	cubeMap.bind();
	renderFbo.begin();
	ofEnableDepthTest();
	ofEnableAlphaBlending();
	//ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofClear(0);
	cam.begin();
	//ofDrawAxis(500);
	renderPass.begin();
	

	//Debug-Light
	ofMatrix4x4 model;
	view = ofGetCurrentViewMatrix();
	proj = cam.getProjectionMatrix();
	model.setTranslation(lightPos);
	renderPass.setUniform1i("type", 1);
	renderPass.setUniformMatrix4f("model", model);
	renderPass.setUniformMatrix4f("view", view);
	renderPass.setUniformMatrix4f("proj", proj);
	//ofSpherePrimitive pointLight;
	
	//lightMesh = pointLight.getMesh();
	//lightMesh.draw();
	
	//Room
	ofBoxPrimitive room;
	room.set(2000.0);
	model.setTranslation(0.0, 0.0, 0.0);
	renderPass.setUniformMatrix4f("model", model);
	renderPass.setUniform1i("type", 2);
	lightMesh = room.getMesh();
	//lightMesh.draw();
	
	
	//Quad-RemderTexture
	renderPass.setUniform1i("type", 0);
	renderPass.setUniformTexture("normalTex", calcNormalFbo.getTexture(0), 1);
	renderPass.setUniformTexture("thicknessTex", thicknessFbo.getTexture(0), 3);
	renderPass.setUniformTexture("positionTex", depthFbo.getTexture(1), 2);

	
	calcNormalPass.setUniformMatrix4f("invProjectionMatrix", proj.getInverse());
	calcNormalPass.setUniform2f("_screenScale", ofVec2f(ofGetWidth(), ofGetHeight()));
	renderPass.setUniform3f("_camPos", cam.getPosition());
	renderPass.setUniform1f("alphaCoef", alphaCoef);
	
	//Lightingcolor
	renderPass.setUniform3f("_albedoColor", ofVec3f(albedoColor.get().r, albedoColor.get().g, albedoColor.get().b));
	renderPass.setUniform3f("_specularColor", ofVec3f(specularColor.get().r, specularColor.get().g, specularColor.get().b));
	renderPass.setUniform3f("_ambientColor", ofVec3f(ambientColor.get().r, ambientColor.get().g, ambientColor.get().b));
	renderPass.setUniform1f("_shininess", shininess);

	renderPass.setUniform3f("_lightPos", ofVec3f(lightPos).normalize());
	renderPass.setUniform3f("_lightCoef", lightCoef);
	renderPass.setUniform3f("_absorbK", absorbK);
	renderPass.setUniform1i("_renderMode", renderMode);
	quad.draw();
	
	cam.end();
	

	renderPass.end();
	renderFbo.end();
	ofDisableBlendMode();
	cubeMap.unbind();
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0.0, 0.0, 0.0);
	//ofEnableAlphaBlending();


	renderFbo.draw(0.0, 0.0);
	//g_buffer.draw(0.0, 0.0);
	ofDisableDepthTest();
	gui.draw();
	
	
	if (isPreview) {
		//cout << g_buffer.getTexture(0).getTextureData().textureID << endl;
		//mrtViewer->preview(depthFbo, thicknessFbo, calcNormalFbo, g_buffer);
		mrtViewer->preview(depthFbo, thicknessFbo, calcNormalFbo, g_buffer);
		//mrtViewer->preview(calcNormalFbo);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ' ') {
		preLoad();
	}
	else if ('p') {
		isPreview = !isPreview;
	}
}
//--------------------------------------------------------------
void ofApp::preLoad() {
	raymarchPass.load("shaders/raymarch.vert", "shaders/raymarch.frag");
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

	renderPass.printActiveUniforms();
}
//--------------------------------------------------------------
void ofApp::initGUI() {
	gui.setup();
	gui.add(particleSize.setup("particleSize", 0.1, 0.0, 1.0));
	gui.add(blurScale.setup("blurScale", 0.08, 0.0, 1.0));
	gui.add(blurDepthFallOff.setup("blurDepthFallOff", 10.0, 0.0, 30.0));
	gui.add(nearClip.setup("nearClip", 0.3, 0.1, 10.0));
	gui.add(farClip.setup("farClip", 50.0, 10.0, 500.0));
	gui.add(alphaCoef.setup("alphaCoef", 0.1, 0.0, 1.0));
	gui.add(lightPos.set("lightPos", ofVec3f(40.0, -200, 114.0), ofVec3f(-200.0, -200.0, -200.0), ofVec3f(200.0, 200.0, 200.0)));
	gui.add(lightCoef.setup("_lightCoef", ofVec3f(0.1, 0.1, 0.2), ofVec3f(.0, .0, .0), ofVec3f(1.0, 1.0, 1.0)));
	gui.add(absorbK.setup("absorbK", ofVec3f(0.34, 0.22, 0.10), ofVec3f(.0, .0, .0), ofVec3f(1.0, 1.0, 1.0)));
	gui.add(renderMode.setup("renderMode", 1, 0, 2));

	gui.add(albedoColor.set("AlbedoColor", ofFloatColor(0.3, 0.45, 0.45), ofFloatColor(0.0, 0.0, 0.0), ofFloatColor(1.0, 1.0, 1.0)));
	gui.add(specularColor.set("SpecularColor", ofFloatColor(1.0, 1.0, 1.0), ofFloatColor(0.0, 0.0, 0.0), ofFloatColor(1.0, 1.0, 1.0)));
	gui.add(ambientColor.set("AmbientColor", ofFloatColor(0.0, 0.03, 0.03), ofFloatColor(0.0, 0.0, 0.0), ofFloatColor(1.0, 1.0, 1.0)));
	gui.add(shininess.set("Shininess", 2.0, 2.0, 200.0));
	
	
}
//--------------------------------------------------------------
void ofApp::initFbo() {

	GLint num_tex_units;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &num_tex_units);
	cout << "Max texture units = " << num_tex_units << endl;


	ofFbo::Settings fboSetting;


	fboSetting.width = ofGetWidth();
	fboSetting.height = ofGetHeight();
	fboSetting.numColorbuffers = 2;
	fboSetting.useDepth = true;
	fboSetting.useStencil = true;
	fboSetting.depthStencilAsTexture = true;
	fboSetting.depthStencilInternalFormat = GL_DEPTH_COMPONENT;
	fboSetting.internalformat = GL_RGBA32F;
	fboSetting.wrapModeHorizontal = GL_CLAMP_TO_EDGE;
	fboSetting.wrapModeVertical = GL_CLAMP_TO_EDGE;
	fboSetting.minFilter = GL_LINEAR;
	fboSetting.maxFilter = GL_LINEAR;
	
	depthFbo.allocate(fboSetting);
	cout << "depthFbo depth : " << depthFbo.getDepthTexture().getTextureData().textureID << endl;
	cout << "depthFbo 0: " << depthFbo.getTexture(0).getTextureData().textureID << endl;
	cout << "depthFbo 1: " << depthFbo.getTexture(1).getTextureData().textureID << endl;
	cout << "------------" << endl;


	fboSetting.numColorbuffers = 1;
	blurFbo1.allocate(fboSetting);
	cout << "blur1Fbo DepthTex: " << blurFbo1.getDepthTexture().getTextureData().textureID << endl;
	cout << "blur1Fbo : " << blurFbo1.getTexture(0).getTextureData().textureID << endl;
	cout << "------------" << endl;

	blurFbo2.allocate(fboSetting);
	cout << "blur2Fbo DepthTex: " << blurFbo2.getDepthTexture().getTextureData().textureID << endl;
	cout << "blur2Fbo : " << blurFbo2.getTexture(0).getTextureData().textureID << endl;
	cout << "------------" << endl;

	calcNormalFbo.allocate(fboSetting);
	cout << "normalFbo DepthTex: " << calcNormalFbo.getDepthTexture().getTextureData().textureID << endl;
	cout << "normalFbo : " << calcNormalFbo.getTexture(0).getTextureData().textureID << endl;
	cout << "------------" << endl;

	thicknessFbo.allocate(fboSetting);
	cout << "thickness Depth : " << thicknessFbo.getDepthTexture().getTextureData().textureID << endl;
	cout << "thickness : " << thicknessFbo.getTexture(0).getTextureData().textureID << endl;
	cout << "------------" << endl;

	renderFbo.allocate(fboSetting);
	cout << "renderFbo Depth: " << renderFbo.getDepthTexture().getTextureData().textureID << endl;
	cout << "renderFbo : " << renderFbo.getTexture(0).getTextureData().textureID << endl;
	cout << "------------" << endl;

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
void ofApp::initGBuffer() {
	/*
	vector<GLint> formats = { GL_RGBA32F};
	ofFbo::Settings settings;
	settings.width = ofGetWidth();
	settings.height = ofGetHeight();
	settings.textureTarget = GL_TEXTURE_2D;
	settings.wrapModeHorizontal = GL_CLAMP_TO_EDGE;
	settings.wrapModeVertical = GL_CLAMP_TO_EDGE;
	settings.minFilter = GL_NEAREST;
	settings.maxFilter = GL_NEAREST;
	settings.numColorbuffers = 1;
	//settings.numColorbuffers = 2;
	settings.colorFormats = formats;
	settings.numSamples = 4;
	settings.useDepth = true;
	settings.useStencil = true;
	*/
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
	g_buffer.allocate(fboSetting);
	g_buffer.checkStatus();


	cout << "gBuffer Depth: " << g_buffer.getDepthTexture().getTextureData().textureID << endl;
	cout << "gBuffer : " << g_buffer.getTexture(0).getTextureData().textureID << endl;

	quad_raymarch.addVertex(ofVec3f(-1.0, -1.0));
	quad_raymarch.addVertex(ofVec3f(-1.0, 1.0));
	quad_raymarch.addVertex(ofVec3f(1.0, 1.0));
	quad_raymarch.addVertex(ofVec3f(1.0, -1.0));

	quad_raymarch.addTexCoord(ofVec2f(.0, .0));
	quad_raymarch.addTexCoord(ofVec2f(.0, 1.0));
	quad_raymarch.addTexCoord(ofVec2f(1.0, 1.0));
	quad_raymarch.addTexCoord(ofVec2f(1.0, .0));

	quad_raymarch.addIndex(0);
	quad_raymarch.addIndex(1);
	quad_raymarch.addIndex(2);

	quad_raymarch.addIndex(2);
	quad_raymarch.addIndex(3);
	quad_raymarch.addIndex(0);
}