#include "Sph.h"

Sph::Sph()
{

}

Sph::Sph(int particleNum) {
	texRes = (int)ceil(sqrt(particleNum));
	cout << "tesRes : " << texRes << endl;
	posBuffer.allocate(texRes, texRes, GL_RGB32F, 1);

	initPosProg.load("shaders/passVert.vert", "shaders/initPosition.frag");
	previewProg.load("shaders/viewPort.vert", "shaders/preview.frag");

	ofDisableArbTex();
	quad.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
	ofVec2f size = ofVec2f(1.0, 1.0);
	ofVec2f texCoord = ofVec2f(1.0, 1.0);
	quad.addVertex(ofVec3f(size.x, size.y, 0.0));
	quad.addTexCoord(ofVec2f(texCoord.x, texCoord.y));
	quad.addVertex(ofVec3f(size.x, -size.y, 0.0));
	quad.addTexCoord(ofVec2f(texCoord.x, 0.0f));
	quad.addVertex(ofVec3f(-size.x, -size.y, 0.0));
	quad.addTexCoord(ofVec2f(0.0f, 0.0f));
	quad.addVertex(ofVec3f(-size.x, size.y, 0.0));
	quad.addTexCoord(ofVec2f(0.0f, texCoord.y));

}




Sph::~Sph()
{
}


void Sph::initBuf() {
	
	if (!isInit) {
		initPosBuf();
	}

}
void Sph::initPosBuf() {
	posBuffer.dst->begin();
	initPosProg.begin();
	ofClear(0.0);
	posBuffer.src->draw(0.0, 0.0);

	initPosProg.end();
	posBuffer.dst->end();

	posBuffer.swap();
}

void Sph::preview() {

	posBuffer.dst->begin();
	initPosProg.begin();
	ofClear(0.0);
	posBuffer.src->draw(0.0, 0.0);

	initPosProg.end();
	posBuffer.dst->end();


	posBuffer.swap();
	previewProg.begin();
	int id = posBuffer.src->getTexture().getTextureData().textureID;
	previewProg.setUniformTexture("tex0", posBuffer.src->getTexture(0), id);
	quad.draw();
	previewProg.end();
}