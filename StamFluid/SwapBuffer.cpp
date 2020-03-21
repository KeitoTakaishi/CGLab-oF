#include "SwapBuffer.h"


SwapBuffer::SwapBuffer()
{

}

SwapBuffer::SwapBuffer(int width, int height, int _internalformat=GL_RGB32F) {
	ofFbo::Settings fboSetting;
	fboSetting.width = width;
	fboSetting.height = height;
	fboSetting.numColorbuffers = 3;//vel, divergence, pressure
	fboSetting.useDepth = false;
	fboSetting.internalformat = _internalformat;
	fboSetting.wrapModeHorizontal = GL_CLAMP_TO_EDGE;
	fboSetting.wrapModeVertical = GL_CLAMP_TO_EDGE;
	fboSetting.minFilter = GL_LINEAR;
	fboSetting.maxFilter = GL_LINEAR;

	for (int i = 0; i < 2; i++) {
		fbo[i].allocate(fboSetting);
	}

	clear();

	ch = 3;
	float* data;
	data = new float[width * height * ch];
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			
			auto i = x + width * y;
			//data[i * ch] = (float)(x) / (float)width;
			data[i * ch] = 0.0;
			data[i * ch + 1] = 0.0;
			data[i * ch + 2] = 0.0;
		}
	}
	fbo[0].getTexture(0).loadData(data, width, height, GL_RGB);
	delete[] data;


	data = new float[width * height * ch];
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {

			auto i = x + width * y;
			//data[i * ch] = (float)(x) / (float)width;
			data[i * ch] = 0.0;
			data[i * ch + 1] = 0.0;
			data[i * ch + 2] = 0.0;
		}
	}
	fbo[0].getTexture(1).loadData(data, width, height, GL_RGB);
	delete[] data;

	src = &fbo[0];
	dst = &fbo[1];
}


SwapBuffer::~SwapBuffer()
{
	
}

void SwapBuffer::clear() {
	for (int i = 0; i < 2; i++) {
		fbo[i].begin();
		ofClear(0, 255);
		fbo[i].end();
	}
}

void SwapBuffer::swap() {
	auto temp = fbo[1];
	fbo[1] = fbo[0];
	fbo[0] = temp;

	src = &fbo[0];
	dst = &fbo[1];
}
