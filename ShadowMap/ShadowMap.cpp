#include "ShadowMap.h"



ShadowMap::ShadowMap()
{
	nearClip = 100;
	farClip = 6000.0;
	shadowShader.load("shaders/shadow");
}


ShadowMap::~ShadowMap()
{
}

void ShadowMap::initFbo() {
	ofDisableArbTex();
	ofFbo::Settings fboSettings;
	fboSettings.width = ofGetWidth();
	fboSettings.height = ofGetHeight();
	fboSettings.textureTarget = GL_TEXTURE_2D;
	fboSettings.internalformat = GL_R8;
	fboSettings.useDepth = true;
	fboSettings.depthStencilAsTexture = true;
	fboSettings.useStencil = true;
	fboSettings.minFilter = GL_LINEAR;
	fboSettings.maxFilter = GL_LINEAR;
	fboSettings.wrapModeHorizontal = GL_CLAMP_TO_EDGE;
	fboSettings.wrapModeVertical = GL_CLAMP_TO_EDGE;
	fbo.allocate(fboSettings);


	biasMatrix = ofMatrix4x4(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);

}

void ShadowMap::setVirtualCam() {
	//light(cam)
	virtualCam.disableOrtho();
	virtualCam.setupPerspective();
	virtualCam.lookAt(ofVec3f(0.0, 0.0, 0.0), ofVec3f(0.0, 1.0, 0.0));
	virtualCam.setPosition(1000, 1000, 0);
	virtualCam.setNearClip(nearClip);
	virtualCam.setFarClip(farClip);
	virtualCam.setFov(90);
	virtualCam.setForceAspectRatio(1.0);
}

void ShadowMap:: beginDepth() {
	fbo.begin();
	ofClear(0, 0, 0, 255);
	virtualCam.begin();
}
void ShadowMap::endDepth() {
	virtualCam.end();
	fbo.end();
}

void ShadowMap::begin(ofCamera& mainCam) {
	/*
	ofMatrix4x4 view, projection;
	view = virtualCam.getModelViewMatrix();
	projection = virtualCam.getProjectionMatrix();
	ofMatrix4x4 inverseCameraMatrix = ofMatrix4x4::getInverseOf(mainCam.getModelViewMatrix());
	ofMatrix4x4 shadowTransMatrix = inverseCameraMatrix * view * projection * biasMatrix;
	
	
	
	shadowShader.begin();
	mainCam.begin();
	shadowShader.setUniformMatrix4f("lightSpaceMatrix", shadowTransMatrix);
	//shadowShader.setUniformMatrix4f("biasMatrix", biasMatrix);
	//shadowShader.setUniformMatrix4f("projMatrix", projection);
	//shadowShader.setUniformMatrix4f("viewMatrix", view);
	//shadowShader.setUniformMatrix4f("invCamMatrix", inverseCameraMatrix);

	shadowShader.setUniform3f("lightPosition", virtualCam.getGlobalPosition());
	shadowShader.setUniformMatrix4f("viewMatrix", ofGetCurrentViewMatrix());
	shadowShader.setUniformTexture("depthMap", fbo.getDepthTexture(), 1);
	*/

	ofMatrix4x4 view, projection;
	view = virtualCam.getModelViewMatrix();
	projection = virtualCam.getProjectionMatrix();
	ofMatrix4x4 inverseCameraMatrix = ofMatrix4x4::getInverseOf(mainCam.getModelViewMatrix());
	ofMatrix4x4 shadowTransMatrix = inverseCameraMatrix * view * projection * biasMatrix;

	mainCam.begin();
	shadowShader.begin();

	shadowShader.setUniformMatrix4f("lightSpaceMatrix", shadowTransMatrix);
	shadowShader.setUniformTexture("depthMap", fbo.getDepthTexture(), 1);

	cout << virtualCam.getGlobalPosition() << endl;
	shadowShader.setUniform3f("lightPosition", virtualCam.getGlobalPosition());
	shadowShader.setUniformMatrix4f("viewMatrix", ofGetCurrentViewMatrix());
}

void ShadowMap::end(ofCamera& mainCam) {
	mainCam.end();
	shadowShader.end();
}

void ShadowMap::debugLight() {
	ofDrawSphere(virtualCam.getGlobalPosition(), 50);
}