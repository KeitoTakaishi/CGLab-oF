//https://gist.github.com/gyohk/89f274082553c58722cc6b1352a047d7
#version 400

uniform int type;//RenderTexture of Geometry


uniform sampler2DRect normalTex;
uniform sampler2DRect thicknessTex;
uniform sampler2DRect positionTex;
uniform sampler2DRect rayMarchingTex;


uniform samplerCube	EnvMap;

uniform mat4 invProjectionMatrix;
uniform mat4 viewMatrix;
uniform vec2 _screenScale;

//Lighting===============
uniform vec3 _lightPos;
uniform vec3 _camPos;

uniform vec3 _ambientColor;
uniform vec3 _albedoColor;
uniform vec3 _specularColor;
uniform float _shininess;
uniform float alphaCoef;
//for Specular
uniform vec3 _absorbK;
uniform vec3 _lightCoef;
uniform int _renderMode;
//Raymarching

uniform float _fluidTransparent;
uniform float _transparent;

in vec2 vTexCoord;
in vec3 vNormal;
out vec4 fragOut;

//0.0 ~ 1.0 -> -1.0 ~ 1.0
vec3 center_zero_Normalize(vec3 v){
    return v * 2.0 - vec3(1.0);
}


/*
uGeomTypeでgeommetryの種類を分けてレンダリング
*/
void main(){
    if(type == 0){
        //EyeSpaceで座標を復元
        //vec3 viewPos = getViewPos(vTexCoord);
 
        vec2 uv = vTexCoord;
        //vec3 normal = texture(depthTex, vTexCoord*vec2(1024.0, 768.0)).xyz;
        //quadにuvを0.0 ~ 1.0で設定しているので定数倍する
        vec3 normal = texture(normalTex, vTexCoord*_screenScale).xyz;
        float thickness = texture(thicknessTex, vTexCoord*_screenScale).x;
        vec3 viewPos = texture(positionTex, vTexCoord*_screenScale).xyz;
        vec3 raymarching = texture(rayMarchingTex, vTexCoord*_screenScale).xyz;

        if(length(raymarching) == 0.0){
            discard;
        }

        //normal.y = 1.0 -normal.y;
        normal = center_zero_Normalize(normal);
        
        vec3 s = normalize(_lightPos);
        vec3 v = normalize(-1.0*viewPos);
        vec3 r = reflect(-s, normal);

        float sDotN = max(dot(s, normal), 0.0);
        float diffuse =  sDotN;
        // float diffuse  = clamp(dot(normal, _lightPos), 0.0, 1.0);
        //vec3 halfV = normalize(_lightPos);
        //float specular = pow(clamp(dot(normal, _lightPos), 0.0, 1.0), 20.0);

        float specular = 0.0;
        if(sDotN > 0.0){
            specular = pow(max(dot(r, v), 0.0), _shininess);
        }


        //absorbColor
        float _d = clamp(thickness, 0.0, 50.0);
        vec3 absorbColor = vec3(exp(-_absorbK.x * _d), exp(-_absorbK.y * _d), exp(-_absorbK.z * _d));
        
        if(_renderMode == 0){
            fragOut = vec4(vec3(raymarching), _transparent);
        }else if(_renderMode == 1){
            vec4 lighting = vec4(_albedoColor * diffuse + _specularColor * specular + _ambientColor, 0.5);
            vec4 absorb = vec4(absorbColor, _d * 1.0);
            fragOut = lighting + absorb;
            //fragOut = lighting;
        }else if(_renderMode == 2){
            vec4 lighting = vec4(_albedoColor * diffuse + _specularColor * specular + _ambientColor, _fluidTransparent);
            vec4 absorb = vec4(absorbColor, _d * 1.0);
            vec4 fluidColor =  absorb + lighting;
            //fluidColor.rgb *= 0.8;

            if(thickness == 0.0){
                //raymarchi
                fragOut = vec4(vec3(raymarching * 0.5), _transparent);
                //fragOut = vec4(0.0, 0.0, 0.0, 1.0);
            }else{
                //fluid
                //fragOut = vec4(fluidColor.rgb, _fluidTransparent) + vec4(vec3(raymarching * 0.5), _transparent);
                fragOut = vec4(fluidColor);
            }
            
            
            //fragOut = vec4(vec3(_albedoColor * diffuse + _specularColor * specular + _ambientColor), _fluidTransparent) + vec4(vec3(raymarching), _transparent);
            //fragOut = vec4(vec3(_albedoColor * diffuse + _specularColor * specular + _ambientColor), 1.0-_transparent);
            //fragOut = vec4(vec3(raymarching), _transparent);
        }
    }else if(type == 1){
        //Geometry
        fragOut = vec4(1.0, 0.0, 0.0, 1.0);
    }else if(type == 2){
        fragOut = vec4(texture(EnvMap, normalize(vNormal).rgb, 1.0));
        //fragOut = vec4(1.0, 0.0, 0.0, 1.0);
    }
}