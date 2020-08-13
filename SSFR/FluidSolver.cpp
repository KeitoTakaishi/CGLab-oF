#include "FluidSolver.h"
FluidSolver::FluidSolver()
{
	range = ofVec3f(10.0, 10.0, 10.0);
	particleNum = 30000;
	particle.setMode(OF_PRIMITIVE_POINTS);
}

FluidSolver::~FluidSolver()
{
}

void FluidSolver::init() {
	shader.load("shaders/particleKernel.vert", "shaders/particleKernel.frag");
	ofLogNotice("ParitcleKernelShader : " + ofToString(shader.getProgram()));

	char buf[MAX_PATH];
	// カレントディレクトリを取得
	GetCurrentDirectoryA(MAX_PATH, buf);
	cout << buf << endl;


	//string fileName = "out50_0017.bsd";
	//string fileName = "out10_0050.bsd";
	//string fileName = "out20_0040.bsd";
	string fileName = "out30_0025.bsd";
	
	//out30_0030
	//out45_0020
	//out50_0017
	string _ = "/data/files/";

	string path = buf + _ + fileName;



	ifstream fin(path, ios::in | ios::binary);
	if (!fin) {	// ファイルオープン失敗してたら
		cout << "粒子点群ファイルがオープンされませんでした." << endl;
		//return 1;
	}
	else {
		cout << "success" << endl;
	}

	int numParticles;
	fin.read(reinterpret_cast<char*>(&numParticles), sizeof(int)); // 粒子数
	cout << "Particle amount: " << numParticles << endl;	// 粒子数の出力

	
	posParticles = new ofVec3f[numParticles];	// 配列確保
	fin.read(reinterpret_cast<char*>(posParticles), numParticles * sizeof(ofVec3f)); // 粒子座標

	
	/*
	for (int i = 0; i < numParticles; i++) {
		cout << i << " : ";
		cout << posParticles[i] << endl;
	}
	*/
	

	for (int i = 0; i < numParticles; i++) {
	//for (int i = 0; i < particleNum; i++) {
		//ofVec3f p = ofVec3f(ofRandom(-range.x, range.x), ofRandom(-range.y, range.y), ofRandom(-range.z, range.z));
		ofVec3f p = ofVec3f(posParticles[i].x - 0.5, -1.0 * (posParticles[i].y - 0.5), posParticles[i].z - 0.5)  * 3.0f;
		pos.push_back(p);
		vel.push_back(ofVec3f(0.0f, ofRandom(2.0f), 0.0));
		particle.addVertex(p);
	}
}


void FluidSolver::update(ofEasyCam& cam) {
	
	for (int i = 0; i < particleNum; i++) {
		pos[i] -= vel[i] * 0.05;
		if (pos[i].y < -range.y) pos[i].y = range.y;
		particle.setVertex(i, pos[i]);
	}
	

	/*
	cam.begin();
	shader.begin();
	ofMatrix4x4 model;
	model.makeIdentityMatrix();
	ofMatrix4x4 view = ofGetCurrentViewMatrix();
	ofMatrix4x4 proj = cam.getProjectionMatrix();
	shader.setUniformMatrix4f("model", model);
	shader.setUniformMatrix4f("view", view);
	shader.setUniformMatrix4f("proj", proj);
	shader.setUniform1f("_time", ofGetElapsedTimef());
	particle.draw(OF_MESH_FILL);
	shader.end();
	cam.end();
	*/
}

void FluidSolver::draw() {
	particle.draw(OF_MESH_FILL);
}