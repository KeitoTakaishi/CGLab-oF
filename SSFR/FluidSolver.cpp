#include "FluidSolver.h"
FluidSolver::FluidSolver()
{
	range = ofVec3f(50.0, 25.0, 50.0);
	particleNum = 50000;
	particle.setMode(OF_PRIMITIVE_POINTS);
}

FluidSolver::~FluidSolver()
{
}

void FluidSolver::init() {

	char buf[MAX_PATH];
	// カレントディレクトリを取得
	GetCurrentDirectoryA(MAX_PATH, buf);
	cout << buf << endl;


	string fileName = "out50_0017.bsd";
	//out10_0050
	//out20_0040
	//out30_0025
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
	

	//for (int i = 0; i < numParticles; i++) {
	for (int i = 0; i < particleNum; i++) {
		ofVec3f p = ofVec3f(ofRandom(-range.x, range.x), ofRandom(-range.y, range.y), ofRandom(-range.z, range.z));
		//ofVec3f p = ofVec3f(posParticles[i].x - 0.5, -1.0 * (posParticles[i].y - 0.5), posParticles[i].z - 0.5)  * 300.0f;
		pos.push_back(p);
		vel.push_back(ofVec3f(0.0f, ofRandom(2.0f), 0.0));
		particle.addVertex(p);
	}
}


void FluidSolver::update() {
	
	
	for (int i = 0; i < particleNum; i++) {
		pos[i] -= vel[i];
		if (pos[i].y < -150.0) pos[i].y = 150.0;
		particle.setVertex(i, pos[i]);
	}
	
	
}

void FluidSolver::draw() {
	particle.draw(OF_MESH_FILL);
}