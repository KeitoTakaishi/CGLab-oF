#include "FluidSolver.h"



FluidSolver::FluidSolver()
{
}


FluidSolver::~FluidSolver()
{
}

void FluidSolver::loadFile() {

	
	cout << "----------------------------" << endl;
	cout << "Load Particle Data" << endl;
	cout << "----------------------------" << endl;

 	char buf[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, buf);
	cout << "CurDir : " << buf << endl;

	//string fileName = "out50_0017.bsd";
	//string fileName = "out10_0050.bsd";
	//string fileName = "out20_0040.bsd";
	string fileName = "out30_0025.bsd";
	string root = "/data/ParticleData/";
	string path = buf + root + fileName;


	ifstream fin(path, ios::in | ios::binary);
	if (!fin) {	
		cout << "Can not Open" << endl;
	}
	else {
		cout << "Success Load Particle Data" << endl;
	}

	fin.read(reinterpret_cast<char*>(&numParticle), sizeof(int));
	cout << "particle num : " << numParticle << endl;


	posParticles = new ofVec3f[numParticle];
	fin.read(reinterpret_cast<char*>(posParticles), numParticle * sizeof(ofVec3f));


	particle.setMode(OF_PRIMITIVE_POINTS);
	float range = 3.0;
	for (int i = 0; i < numParticle; i++) {
		ofVec3f p = ofVec3f(posParticles[i].x - 0.5, -1.0 * (posParticles[i].y - 0.5), posParticles[i].z - 0.5)  * range;
		pos.push_back(p);
		particle.addVertex(p);
	}
}

void FluidSolver::draw() {
	//particle.drawVertices();
	particle.draw(OF_MESH_FILL);
}