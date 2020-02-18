//
//  pingPongBuffer.hpp
//  Fbo-Sample02
//
//  Created by takaishi keito on 2020/02/18.
//

#ifndef pingPongBuffer_hpp
#define pingPongBuffer_hpp

struct pingPongBuffer{
public:
    void allocate(int _width, int _height, int _internalformat = GL_RGBA, int _numColorBuffers = 1){
        ofFbo::Settings fboSettings;
        fboSettings.width  = _width;
        fboSettings.height = _height;
        fboSettings.numColorbuffers = _numColorBuffers;
        fboSettings.useDepth = false;
        fboSettings.internalformat = _internalformat;    // Gotta store the data as floats, they won't be clamped to 0..1
        fboSettings.wrapModeHorizontal = GL_CLAMP_TO_EDGE;
        fboSettings.wrapModeVertical = GL_CLAMP_TO_EDGE;
        fboSettings.minFilter = GL_NEAREST; // No interpolation, that would mess up data reads later!
        fboSettings.maxFilter = GL_NEAREST;
        for(int i = 0; i < 2; i++){
            FBOs[i].allocate(fboSettings);
        }
        
        clear();
        
        flag = 0;
        swap();
        flag = 0;
    }
    
    void swap(){
        src = &(FBOs[(flag)%2]);
        dst = &(FBOs[++(flag)%2]);
    }
    
    void clear(){
        for(int i = 0; i < 2; i++){
            FBOs[i].begin();
            ofClear(0,255);
            FBOs[i].end();
        }
    }
    
    
    ofFbo& operator[] (int n){
        return FBOs[n];
    }
    ofFbo *src;
    ofFbo *dst;
private:
    ofFbo FBOs[2];
    int flag;
};

#endif /* pingPongBuffer_hpp */
