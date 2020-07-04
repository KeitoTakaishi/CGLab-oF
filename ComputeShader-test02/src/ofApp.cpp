#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	firstKernel.loadCompute("shaders/firstKernel.glsl");
	finalKernel.loadCompute("shaders/finalKernel.glsl");
	render.load("shaders/render.vert", "shaders/render.frag");
	numParticle = 1024 * 256;
	vector<Particle> particleData;
	vector<Particle> particleRWData;
	vector<Buffer1> buffer1Data;

	particleData.resize(numParticle);
	particleRWData.resize(numParticle);
	buffer1Data.resize(numParticle);

	float r = 150;
	
	for (int i = 0; i < numParticle; i++) {
		particleData[i].pos = ofVec4f(ofRandom(-r, r), ofRandom(-r, r), ofRandom(-r, r), 0.0);
		particleData[i].vel = ofVec4f(0.0, 0.0, 0.0, 0.0);
		particleData[i].col = ofVec4f(1.0, 1.0, 1.0, 1.0);

		particleRWData[i].pos = ofVec4f(0.0, 0.0, 0.0, 0.0);
		particleRWData[i].vel = ofVec4f(0.0, 0.0, 0.0, 0.0);
		particleRWData[i].col = ofVec4f(1.0, 1.0, 1.0, 1.0);

		particle.addVertex(
			ofVec3f(
				particleData[i].pos.x,
				particleData[i].pos.y,
				particleData[i].pos.z
			));
		particle.addColor(ofFloatColor(
			particleData[i].col.x, 
			particleData[i].col.y,
			particleData[i].col.z,
			particleData[i].col.w));

		buffer1Data[i].data = 0.0;
	}
	cout << particle.getVertices().size() << endl;
	particleBuffer.allocate(particleData, GL_DYNAMIC_DRAW);
	particleRWBuffer.allocate(particleRWData, GL_DYNAMIC_DRAW);
	buffer1.allocate(buffer1Data, GL_DYNAMIC_DRAW);
	//particleBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 0);
	//particleRWBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 1);
	//cout << "particleBuffer : " << particleBuffer.getId() << endl;
	//cout << "particleRWBuffer : " << particleRWBuffer.getId() << endl;

	wallSize = ofVec3f(500, 500, 500);
	ofSetBackgroundColor(30);
}

//--------------------------------------------------------------
void ofApp::update(){

}

void ofApp::drawUI() {
	ofPushStyle();
	ofSetColor(255.0, 255.0, 255.0, 50.0);
	ofFill();
	ofRect(0.0, 0.0, 300, 120);
	ofSetColor(255.0, 255.0, 255.0);
	ofDrawBitmapString("*ComputeShader-Debug", 25.0, 25.0);
	ofDrawBitmapString("*>>Research ofBufferObject", 25.0, 45.0);
	ofDrawBitmapString("*FPS : " + to_string(ofGetFrameRate()), 25.0, 65.0);
	ofDrawBitmapString("*Particle-Num : " + to_string(numParticle), 25.0, 85.0);
	ofDrawBitmapString("*GLSL-4.5ver", 25.0, 105.0);

	ofPopStyle();
}
//--------------------------------------------------------------
void ofApp::draw(){
	drawUI();
	
	float time = ofGetElapsedTimef();
	//cam.setPosition(sin(time * 0.2) * 800, 100, cos(time * 0.2) * 800);
	//cam.lookAt(ofVec3f(0, 0, 0));

	cam.begin();
	glEnable(GL_DEPTH_TEST);

	ofNoFill();
	ofDrawBox(0, 0, 0, wallSize.x, wallSize.y, wallSize.z);


	//compute
	//first
	buffer1.bind(GL_SHADER_STORAGE_BUFFER);
	buffer1.bindBase(GL_SHADER_STORAGE_BUFFER, 0);
	firstKernel.begin();
	firstKernel.dispatchCompute(numParticle / BLOCK_SIZE, 1, 1);
	firstKernel.end();
	buffer1.unbindBase(GL_SHADER_STORAGE_BUFFER, 0);
	buffer1.unbind(GL_SHADER_STORAGE_BUFFER);
	
	//ここで一回buffer1の値が更新されているのか確認したい．．．
	

	//final
	particleBuffer.bind(GL_SHADER_STORAGE_BUFFER);
	particleBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 0);
	particleRWBuffer.bind(GL_SHADER_STORAGE_BUFFER);
	particleRWBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 1);
	buffer1.bind(GL_SHADER_STORAGE_BUFFER);
	buffer1.bindBase(GL_SHADER_STORAGE_BUFFER, 2);
	

	finalKernel.begin();
	//firstKernel.dispatchCompute(floor(numParticle / BLOCK_SIZE), 1, 1);
	finalKernel.dispatchCompute(numParticle/BLOCK_SIZE, 1, 1);
	finalKernel.end();
	
	particleBuffer.unbindBase(GL_SHADER_STORAGE_BUFFER, 0);
	particleBuffer.unbind(GL_SHADER_STORAGE_BUFFER);
	particleRWBuffer.unbindBase(GL_SHADER_STORAGE_BUFFER, 1);
	particleRWBuffer.unbind(GL_SHADER_STORAGE_BUFFER);
	buffer1.unbindBase(GL_SHADER_STORAGE_BUFFER, 2);
	buffer1.unbind(GL_SHADER_STORAGE_BUFFER);
	



	//render
	render.begin();
	particleBuffer.bind(GL_SHADER_STORAGE_BUFFER);
	particleBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 0);
	particle.draw(OF_MESH_POINTS);
	particleBuffer.unbindBase(GL_SHADER_STORAGE_BUFFER, 0);
	particleBuffer.unbind(GL_SHADER_STORAGE_BUFFER);
	render.end();
	

	glDisable(GL_DEPTH_TEST);
	cam.end();

	temp = particleBuffer;
	particleBuffer = particleRWBuffer;
	particleRWBuffer = temp;
}