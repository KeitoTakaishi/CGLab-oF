#version 150
uniform sampler2DRect backFaceDepthTex;
uniform sampler2DRect frontFaceDepthTex;


uniform float mode;
uniform vec2 camPrames;
uniform vec2 screenScale;
uniform vec3 maxCol;
uniform float thicknessCoef;
in vec2 vTexCoord;
out vec4 outputColor;


float LinearizeDepth(float depth)  
{  
    vec2 camClips = vec2(camPrames.x, camPrames.y);
	return (2.0 * camClips.x) / (camClips.y + camClips.x - depth * (camClips.y - camClips.x));  
} 

void main()
{
    float backDepth = texture(backFaceDepthTex, vTexCoord * screenScale).r;
    float frontDepth = texture(frontFaceDepthTex, vTexCoord * screenScale).r;
    
    float d = 100.0;
    
    if(mode == 0.0){
        d = LinearizeDepth(backDepth);
        //outputColor = vec4(vec3( exp(-thicknessCoef * d) ), 1.0);
        outputColor = vec4(vec3(d * thicknessCoef) , 1.0);
        //outputColor = vec4(vec3(1.0) , 1.0);
    }else if(mode == 1.0){
        d = LinearizeDepth(frontDepth);
        //outputColor = vec4(vec3( exp(-thicknessCoef * d) ), 1.0);
         outputColor = vec4(vec3(d * thicknessCoef) , 1.0);
    }else{
        d = LinearizeDepth(backDepth - frontDepth);
        //float _d =  exp(-thicknessCoef * d);
        //vec3 c = mix(vec3(0.0, 0.0, 0.0), maxCol, _d);
        //outputColor = vec4(vec3(log(d * thicknessCoef)) , 1.0);
        outputColor = vec4(vec3(log(d * thicknessCoef)) , 1.0);
    }
    
       
   
}