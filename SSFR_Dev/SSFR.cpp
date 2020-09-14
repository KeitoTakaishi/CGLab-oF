#include "SSFR.h"



SSFR::SSFR()
{
	
}



SSFR::~SSFR()
{
}

void SSFR::init() {
	createFbo();
}

void SSFR::createFbo() {

	//DepthFbo
	ofFboSettings s;
	s.width = ofGetWidth();
	s.height = ofGetHeight();
	s.numColorbuffers = 2;
	s.internalformat = GL_RGBA32F;
	s.wrapModeHorizontal = GL_CLAMP_TO_EDGE;
	s.wrapModeVertical = GL_CLAMP_TO_EDGE;
	s.useDepth = true;
	s.depthStencilInternalFormat = GL_DEPTH_COMPONENT32F;
	s.minFilter = GL_LINEAR;
	s.maxFilter = GL_LINEAR;


	depthFbo.allocate(s);
	s.numColorbuffers = 1;
	thicknessFbo.allocate(s);
	renderFbo.allocate(s);

	
	s.useDepth = false;
	blur1Fbo.allocate(s);
	blur2Fbo.allocate(s);
	normalFbo.allocate(s);
	
	
}

void SSFR::loadShader() {	
	calcDepthPass.setGeometryInputType(GL_POINTS);
	calcDepthPass.setGeometryOutputType(GL_TRIANGLE_STRIP);
	calcDepthPass.setGeometryOutputCount(4);
	calcDepthPass.load("ShadersGL4/pointSprites.vert", "ShadersGL4/calcDepth.frag", "ShadersGL4/depth.geom");
	blurPass.load("ShadersGL4/passThru.vert", "ShadersGL4/blur.frag");
	calcNormalPass.load("ShadersGL4/passThru.vert", "ShadersGL4/calcNormal.frag");
	calcThicknessPass.setGeometryInputType(GL_POINTS);
	calcThicknessPass.setGeometryOutputType(GL_TRIANGLE_STRIP);
	calcThicknessPass.setGeometryOutputCount(4);
	calcThicknessPass.load("ShadersGL4/pointSprites.vert", "ShadersGL4/calcThickness.frag", "ShadersGL4/depth.geom");
	renderSSFRPass.load("ShadersGL4/passThru.vert", "ShadersGL4/render.frag");
	renderSSFRPass.printActiveUniforms();
	
	cubeMapPass.load("ShadersGL4/cubeMap.vert", "ShadersGL4/cubeMap.frag");
	rayMarchingPass.load("ShadersGL4/rayMarching.vert", "ShadersGL4/rayMarching.frag");
}


void SSFR::calcDepthBegin() {
	this->depthFbo.begin();
	ofClear(0);
}

void SSFR::calcDepthEnd() {
	this->depthFbo.end();
}

void SSFR::blurBegin() {
}
void SSFR::blurEnd() {
}
