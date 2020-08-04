//https://gist.github.com/gyohk/89f274082553c58722cc6b1352a047d7
#version 150

uniform int type;//RenderTexture of Geometry

uniform sampler2DRect normalTex;
uniform sampler2DRect thicknessTex;
uniform samplerCube	EnvMap;

//Lighting===============
uniform vec3 _ambientColor;
uniform vec3 _albedoColor;
uniform vec3 _specularColor;

uniform float alphaCoef;
uniform vec3 _lightDir;
//for Specular
uniform vec3 lightPos;
uniform vec3 camPos;
uniform vec3 _absorbK;
uniform vec3 _lightCoef;
uniform int _renderMode;


in vec2 vTexCoord;
in vec3 vNormal;
out vec4 fragOut;



void main(){
    if(type == 0){
        vec2 uv = vTexCoord;
        //vec3 normal = texture(depthTex, vTexCoord*vec2(1024.0, 768.0)).xyz;
        //quadにuvを0.0 ~ 1.0で設定しているので定数倍する
        
        vec3 normal = texture(normalTex, vTexCoord*vec2(1024.0, 768.0)).xyz;
        float thickness = texture(thicknessTex, vTexCoord*vec2(1024.0, 768.0)).x;
        

        //if(normal.x==0.0) discard;
        if(length(normal) == 0.0){
            discard;
        }
        normal = normal * 2.0 - vec3(1.0, 1.0, 1.0);
        
        float diffuse  = clamp(dot(normal, _lightDir), 0.0, 1.0);
        
        vec3 halfV = normalize(_lightDir);
        float specular = pow(clamp(dot(normal, _lightDir), 0.0, 1.0), 20.0);


        //absorbColor
        //float reFitThickness = clamp
        float _d = clamp(thickness, 0.0, 50.0);
        vec3 absorbColor = vec3(exp(-_absorbK.x * _d), exp(-_absorbK.y * _d), exp(-_absorbK.z * _d));
                
        if(_renderMode == 0){
            //fragOut = vec4(_d * 0.1);   
            fragOut = vec4(1.0);   
        }else if(_renderMode == 1){
            fragOut = vec4(vec3(_albedoColor * diffuse + _specularColor * specular + _ambientColor), 0.7); 
        }else if(_renderMode == 2){
            fragOut = vec4(vec3(_albedoColor * diffuse + _specularColor * specular + _ambientColor), thickness * 0.2) 
            + vec4(absorbColor, _d * .1);
        }
    }else if(type == 1){
        //Geometry
        fragOut = vec4(1.0, 0.0, 0.0, 1.0);
    }else if(type == 2){
        fragOut = vec4(texture(EnvMap, normalize(vNormal).rgb, 0.1)) * vec4(1.0, 1.0, 1.0, 0.2);
    }
}