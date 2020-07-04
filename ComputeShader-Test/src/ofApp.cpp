#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0.0);

	/*
	compute.loadCompute("shaders/compute.glsl");
	renderParticle.load("shaders/render.vert", "shaders/render.frag");
	numParticle = 1024 * 1024;
	//numParticle = 128 * 128;
	vector<Particle> particleData;
	vector<ColorData> colorData;
	particleData.resize(numParticle);
	colorData.resize(numParticle);

	for (auto& p : particleData) {
		p.pos.x = ofRandom(-1.0, 1.0);
		p.pos.y = ofRandom(-1.0, 1.0);
		p.pos.z = ofRandom(-1.0, 1.0);
		p.pos.w = 0.0;

		p.vel.x = ofRandom(-5.0, 5.0);
		p.vel.y = ofRandom(-5.0, 5.0);
		p.vel.z = ofRandom(-5.0, 5.0);
		p.vel.w = (int)ofRandom(0.0, 4.0);
		///p.col = ofVec3f(1.0, 0.0, 0.0);
		p.col = ofVec4f(1.0, 0.0, 0.0, 0.0);

		particle.addVertex(ofVec3f(p.pos));
		particle.addColor(ofFloatColor(1.0, 1.0, 1.0, 1.0));
	}

	for (auto& c : colorData) {
		//c.col = ofVec3f(1., 1., 1.);
		c.col = 1.0;
	}

	dataBuffer.allocate(particleData, GL_DYNAMIC_DRAW);
	colorBuffer.allocate(colorData, GL_DYNAMIC_DRAW);
	*/
	wallSize = ofVec3f(500, 500, 500);
	


	//-------------------------------------------------
	solver = new SPH3D_Solver();
	//cout << solver->particleNum << endl;
	solver->init();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

	ofDrawBitmapString(to_string(ofGetFrameRate()), 100.0, 100.0);
	float time = ofGetElapsedTimef();
	cam.setPosition(sin(time * 0.2) * 800, 100, cos(time * 0.2) * 800);
	cam.lookAt(ofVec3f(0, 0, 0));

	cam.begin();
	glEnable(GL_DEPTH_TEST);

	for (int i = 0; i < 4; i++) {
		solver->execute();
	}
	/*
	// Buffer Objectをバインド
	dataBuffer.bind(GL_SHADER_STORAGE_BUFFER);
	dataBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 0);

	colorBuffer.bind(GL_SHADER_STORAGE_BUFFER);
	colorBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 1);

	// Compute shaderで頂点/速度を計算する
	compute.begin();
	compute.setUniform3f("wallSize", wallSize);
	compute.setUniform3f("wallCenter", ofVec3f(0, 0, 0));
	//computeShader側のthreadSizeと要相談
	//compute.dispatchCompute(128, 1, 1);
	compute.dispatchCompute(1024, 1, 1);
	compute.end();

	// 計算結果を基にParticleを描画
	renderParticle.begin();
	renderParticle.setUniform1f("time", time);
	particle.draw(OF_MESH_POINTS);
	renderParticle.end();

	// Buffer Objectをアンバインド
	dataBuffer.unbindBase(GL_SHADER_STORAGE_BUFFER, 0);
	dataBuffer.unbind(GL_SHADER_STORAGE_BUFFER);

	colorBuffer.unbindBase(GL_SHADER_STORAGE_BUFFER, 1);
	colorBuffer.unbind(GL_SHADER_STORAGE_BUFFER);
	
	*/

	ofNoFill();
	ofDrawBox(0, 0, 0, wallSize.x, wallSize.y, wallSize.z);

	glDisable(GL_DEPTH_TEST);
	cam.end();
}

