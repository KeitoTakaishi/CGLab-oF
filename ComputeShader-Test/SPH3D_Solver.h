#pragma once
#include "ofMain.h"

class SPH3D_Solver
{
	public:
		SPH3D_Solver();
		~SPH3D_Solver();

		int particleNum;//256x256


		struct Particle
		{
			ofVec4f pos;
			ofVec4f vel;
		};

		struct Density
		{
			float density;
		};

		struct Press
		{
			float press;
		};


		struct Force
		{
			ofVec4f force;
		};

		ofBufferObject particleDataBuffer;
		ofBufferObject particleDataRWBuffer;
		ofBufferObject particleForceBuffer;
		ofBufferObject particleDensityBuffer;
		ofBufferObject particlePressureBuffer;
		ofBufferObject temp;
		
		//ComputeShader
		ofShader calcDensityKernel;
		ofShader calcPressureKernel;
		ofShader calcForceKernel;
		ofShader calcIntegrateForce;
		int blockSize;

		float h;
		float pressureStiffness;
		float defaultDensity;
		float mass;
		float viscoCoef;//myu
		float wallStiffness;
		float dt;
		float densityWeight;
		float gradPressWeight;
		float lapViscWeight;

		//Shader
		ofShader render;

		ofVboMesh particle;
		int numParticle;


		void init();
		void loadShaders();
		void initParams();
		void initBuffer();
		
		void execute();
};