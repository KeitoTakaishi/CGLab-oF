#include "ofxMRTViewer.h"
ofxMRTViewer::ofxMRTViewer(int _texNum) {
	texNum = _texNum;
	ofVec2f res = ofVec2f(2, 2);
	float h = ofGetHeight() / _texNum;
	panelSize = ofVec2f(h*ofGetWidth() / ofGetHeight(), h);
	cout << "panelSize : " << panelSize << endl;
	for (int i = 0; i < _texNum; i++) {
		panels.push_back(new ofPlanePrimitive(panelSize.x, panelSize.y, res.x, res.y));
	}
	shader.load("shadersGL3/shader");
}

void ofxMRTViewer::preview(ofFbo& _mrt) {
	float w = ofGetWidth();
	for (int i = 0; i < texNum; i++) {
		ofPushMatrix();
		if (i == 0) {
			ofTranslate(w - panels[i]->getWidth() / 2.0, panels[i]->getHeight() / 2.0);
		}
		else {
			ofTranslate(w - panels[i]->getWidth() / 2.0, panels[i]->getHeight()*(i + 0.5));
		}
		shader.begin();
		shader.setUniform1f("n", (float)i);
		shader.setUniformTexture("tex", _mrt.getTexture(i), i);
		panels[i]->draw();
		shader.end();
		ofPopMatrix();
	}
}

void ofxMRTViewer::preview(ofFbo& _normal, ofFbo& _depth, ofFbo& _blurDepth) {
	float w = ofGetWidth();
	for (int i = 0; i < texNum; i++) {
		ofPushMatrix();
		if (i == 0) {
			ofTranslate(w - panels[i]->getWidth() / 2.0, panels[i]->getHeight() / 2.0);
		}
		else {
			ofTranslate(w - panels[i]->getWidth() / 2.0, panels[i]->getHeight()*(i + 0.5));
		}
		shader.begin();
		shader.setUniform1f("n", (float)i);
		shader.setUniform2f("size", ofVec2f(ofGetWidth(), ofGetHeight()));
		if (i == 0) {
			shader.setUniformTexture("tex", _normal.getTexture(0), i);
		}
		else if (i == 1) {
			shader.setUniformTexture("tex", _depth.getTexture(0), i);
		}
		else if (i == 2) {
			shader.setUniformTexture("tex", _blurDepth.getTexture(0), i);
		}
		
		panels[i]->draw();
		shader.end();
		ofPopMatrix();
	}
}