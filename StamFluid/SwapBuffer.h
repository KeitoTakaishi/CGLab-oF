#pragma once
#include "ofMain.h"
class SwapBuffer
{
	public:
		SwapBuffer();
		SwapBuffer(int width, int height, int _internalformat);
		~SwapBuffer();

		void clear();
		void swap();
		
		
		ofFbo* src;
		ofFbo* dst;


	private:
		ofFbo fbo[2];
		int ch = 3;
};