#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetBackgroundColor(0);
    sph = new Sph(numParticles);
    //sph->timeStep();
    
    
    /*
    int sizeY = 5;
    int sizeX = 10;
    vector<vector<float>> v;
    v.resize(sizeY);
    for(int i = 0; i < sizeY; i++){
         v[i].resize(sizeX);
    }
    
    for(int i = 0; i < sizeX; i++){
        for(int j = 0; j < sizeY; j++){
            v[i][j] = j;
        }
    }
    
    for(int i = 0; i < sizeY; i++){
        for(int j = 0; j < sizeX; j++){
            cout << v[i][j] << endl;
        }
        cout << "---------" << endl;
    }
     */
   
}

//--------------------------------------------------------------
void ofApp::update(){
    int cellStatus = 0;
    if (cellStatus == 0) { // エラーがなければ
        cellStatus = sph->timeStep(); // SPH の計算を 1 フレーム進める
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    cam.begin();
    sph->preview();
    cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'c'){
        //sph->timeStep();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
