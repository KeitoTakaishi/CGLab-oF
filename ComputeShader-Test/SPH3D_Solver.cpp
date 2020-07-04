#include "SPH3D_Solver.h"



SPH3D_Solver::SPH3D_Solver()
{
	numParticle = 256 * 256;
	blockSize = 256;
}


SPH3D_Solver::~SPH3D_Solver()
{
	
}

void SPH3D_Solver::init() {
	loadShaders();
	initParams();
	initBuffer();
}

void SPH3D_Solver::loadShaders() {
	calcDensityKernel.loadCompute("shaders/calcDensityKernel.glsl");
	calcPressureKernel.loadCompute("shaders/calcPressrureKernel.glsl");
	calcForceKernel.loadCompute("shaders/calcForceKernel.glsl");
	calcIntegrateForce.loadCompute("shaders/calcIntegrateForce.glsl");
	render.load("shaders/render.vert", "shaders/render.frag");
}

void SPH3D_Solver::initParams() {
	h = .88;
	defaultDensity = 200;
	pressureStiffness = 0.001;
	mass = 0.04;
	viscoCoef = 0.84;
	wallStiffness = 300;
	
	densityWeight = mass * 315.0f / (64.0f * PI * pow(h, 9));
	gradPressWeight = mass * -45.0f / (PI * pow(h, 6));
	lapViscWeight = mass * 45.0f / (PI * pow(h, 6));
	//dt = 1.0 / 60.0;
	dt = 0.01;

	cout << "densityWeight : " << densityWeight << endl;
	cout << "gradPressWeight : " << gradPressWeight << endl;
	cout << "lapViscWeight : " << lapViscWeight << endl;
}

void SPH3D_Solver::initBuffer() {
	vector<Particle> particleData;
	vector<Particle> particleRWData;
	vector<Particle> tempData;
	vector<Density> densityData;
	vector<Press> pressData;
	vector<Force> forceData;

	particleData.resize(numParticle);
	particleRWData.resize(numParticle);
	tempData.resize(numParticle);
	densityData.resize(numParticle);
	pressData.resize(numParticle);
	forceData.resize(numParticle);

	float radius = 10.0;
	for (auto& p : particleData) {
		p.pos.x = ofRandom(-radius, radius);
		p.pos.y = ofRandom(-radius, radius);
		p.pos.z = ofRandom(-radius, radius);
		p.pos.w = 0.0;
		p.vel = ofVec4f(0.0, 0.0, 0.0, 0.0);
	}

	for (auto& p : particleRWData) {
		p.pos = ofVec4f(0.0, 0.0, 0.0, 0.0);
		p.vel = ofVec4f(0.0, 0.0, 0.0, 0.0);
		particle.addVertex(ofVec3f(p.pos));
		particle.addColor(ofFloatColor(1.0, 1.0, 1.0, 1.0));
	}

	for (auto& p : tempData) {
		p.pos = ofVec4f(0.0, 0.0, 0.0, 0.0);
		p.vel = ofVec4f(0.0, 0.0, 0.0, 0.0);
	}
	
	for (int i = 0; i < numParticle; i++) {
		particleRWData[i].pos = ofVec4f(0.0, 0.0, 0.0, 0.0);
		particleRWData[i].vel = ofVec4f(0.0, 0.0, 0.0, 0.0);
		densityData[i].density = 0.0;
		pressData[i].press = 0.0;
		forceData[i].force = ofVec4f(0.0, 0.0, 0.0, 0.0);
	}

	particleDataBuffer.allocate(particleData, GL_DYNAMIC_DRAW);
	particleDataRWBuffer.allocate(particleRWData, GL_DYNAMIC_DRAW);
	temp.allocate(tempData, GL_DYNAMIC_DRAW);
	particleDensityBuffer.allocate(densityData, GL_DYNAMIC_DRAW);
	particlePressureBuffer.allocate(pressData, GL_DYNAMIC_DRAW);
	particleForceBuffer.allocate(forceData, GL_DYNAMIC_DRAW);


}

void SPH3D_Solver::execute() {

	//-------------------------------------------------------------
	//1.calcDensity
	//–§“x‚ÉŠÖ‚µ‚Ä‘‚«ž‚Ý‚ðs‚¤
	{
		particleDataBuffer.bind(GL_SHADER_STORAGE_BUFFER);
		particleDataBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 0);

		particleDensityBuffer.bind(GL_SHADER_STORAGE_BUFFER);
		particleDensityBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 1);	

		calcDensityKernel.begin();
		calcDensityKernel.setUniform1f("densityWeight", densityWeight);
		calcDensityKernel.setUniform1f("h", h);
		calcDensityKernel.setUniform1f("numParticle", numParticle);
		calcDensityKernel.dispatchCompute(numParticle / 256, 1, 1);
		calcDensityKernel.end();

		//cout << particleDataBuffer.getId() << endl;
		//cout << particleDensityBuffer.getId() << endl;

		particleDataBuffer.unbindBase(GL_SHADER_STORAGE_BUFFER, 0);
		particleDataBuffer.unbind(GL_SHADER_STORAGE_BUFFER);

		particleDensityBuffer.unbindBase(GL_SHADER_STORAGE_BUFFER, 1);
		particleDensityBuffer.unbind(GL_SHADER_STORAGE_BUFFER);
	}
	vector<Density> debugData;
	debugData.resize(numParticle);
	//-------------------------------------------------------------
	
	//2.calcPressure
	{
		particleDensityBuffer.bind(GL_SHADER_STORAGE_BUFFER);
		particleDensityBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 0);
		particlePressureBuffer.bind(GL_SHADER_STORAGE_BUFFER);
		particlePressureBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 1);

		calcPressureKernel.begin();
		calcPressureKernel.setUniform1f("h", h);
		calcPressureKernel.setUniform1f("B", pressureStiffness);
		calcPressureKernel.setUniform1f("defaultDensity", defaultDensity);
		calcPressureKernel.dispatchCompute(numParticle / 256, 1, 1);
		calcPressureKernel.end();

		particleDensityBuffer.unbindBase(GL_SHADER_STORAGE_BUFFER, 0);
		particleDensityBuffer.unbind(GL_SHADER_STORAGE_BUFFER);
		particlePressureBuffer.unbindBase(GL_SHADER_STORAGE_BUFFER, 1);
		particlePressureBuffer.unbind(GL_SHADER_STORAGE_BUFFER);
	}
	//-------------------------------------------------------------
	//3.calcForce
	//GL_SHADER_STORAGE_BUFFER
	{
		particleDataBuffer.bind(GL_SHADER_STORAGE_BUFFER);
		particleDataBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 0);


		particleDensityBuffer.bind(GL_SHADER_STORAGE_BUFFER);
		particleDensityBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 1);

		particlePressureBuffer.bind(GL_SHADER_STORAGE_BUFFER);
		particlePressureBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 2);

		particleForceBuffer.bind(GL_SHADER_STORAGE_BUFFER);
		particleForceBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 3);


		calcForceKernel.begin();
		calcForceKernel.setUniform1f("gradPressWeight", gradPressWeight);
		calcForceKernel.setUniform1f("lapViscWeight", lapViscWeight);
		calcForceKernel.setUniform1f("h", h);
		calcForceKernel.setUniform1f("numParticle", numParticle);
		calcForceKernel.setUniform1f("viscCoef", viscoCoef);
		calcForceKernel.dispatchCompute(numParticle / 256, 1, 1);
		calcForceKernel.end();

		particleDataBuffer.unbindBase(GL_SHADER_STORAGE_BUFFER, 0);
		particleDataBuffer.unbind(GL_SHADER_STORAGE_BUFFER);

		particleDensityBuffer.unbindBase(GL_SHADER_STORAGE_BUFFER, 1);
		particleDensityBuffer.unbind(GL_SHADER_STORAGE_BUFFER);

		particlePressureBuffer.unbindBase(GL_SHADER_STORAGE_BUFFER, 2);
		particlePressureBuffer.unbind(GL_SHADER_STORAGE_BUFFER);

		particleForceBuffer.unbindBase(GL_SHADER_STORAGE_BUFFER, 3);
		particleForceBuffer.unbind(GL_SHADER_STORAGE_BUFFER);
	}
	//-------------------------------------------------------------
	//4.IntegrateForce
	particleDataBuffer.bind(GL_SHADER_STORAGE_BUFFER);
	particleDataBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 0);

	particleDataRWBuffer.bind(GL_SHADER_STORAGE_BUFFER);
	particleDataRWBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 1);

	particleForceBuffer.bind(GL_SHADER_STORAGE_BUFFER);
	particleForceBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 2);
	

	calcIntegrateForce.begin();
	calcIntegrateForce.setUniform1f("wallStiffness", wallStiffness);
	calcIntegrateForce.setUniform1f("dt", dt);
	calcIntegrateForce.dispatchCompute(numParticle / 256, 1, 1);
	calcIntegrateForce.end();

	particleDataBuffer.unbindBase(GL_SHADER_STORAGE_BUFFER, 0);
	particleDataBuffer.unbind(GL_SHADER_STORAGE_BUFFER);

	particleDataRWBuffer.unbindBase(GL_SHADER_STORAGE_BUFFER, 1);
	particleDataRWBuffer.unbind(GL_SHADER_STORAGE_BUFFER);

	particleForceBuffer.unbindBase(GL_SHADER_STORAGE_BUFFER, 2);
	particleForceBuffer.unbind(GL_SHADER_STORAGE_BUFFER);
	//-------------------------------------------------------------
	//render
	render.begin();
	particleDataBuffer.bind(GL_SHADER_STORAGE_BUFFER);
	particleDataBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 0);
	particle.draw(OF_MESH_POINTS);
	particleDataBuffer.unbindBase(GL_SHADER_STORAGE_BUFFER, 0);
	particleDataBuffer.unbind(GL_SHADER_STORAGE_BUFFER);
	render.end();
	
	//-------------------------------------------------------------
	//particleDataBuffer.copyTo(temp);
	//particleDataRWBuffer.copyTo(particleDataBuffer);
	//temp.copyTo(particleDataBuffer);
	temp = particleDataBuffer;
	particleDataRWBuffer = particleDataBuffer;
	particleDataBuffer = temp;

}