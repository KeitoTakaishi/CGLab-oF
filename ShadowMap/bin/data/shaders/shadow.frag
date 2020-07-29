#version 330

uniform sampler2D depthMap;

in vec3 normalVarying;
in vec4 positionVarying;


in vec3 v_normalVarying;
in vec4 v_positionVarying;
in vec4 v_shadowCoord;
in vec3 v_lightPosition;//LightPos in ViewSpace 


out vec4 fragColor;


float ShadowCalc(vec4 shadowCoord)
{
    
    vec3 projCoords = shadowCoord.xyz / shadowCoord.w;
    float currentDepth = projCoords.z;//distance from light

    float shadow = 0.0;

    if(currentDepth - 0.0001 < texture(depthMap, projCoords.xy).r) {
        shadow = 1.0;
    }

    if(projCoords.x <= 0.0 || projCoords.y <= 0.0 ||
    projCoords.x >= 1.0 || projCoords.y >= 1.0 ||
    projCoords.z >= 1.0 || projCoords.z <= 0.0) {
        shadow = 1.0;
    }
	return shadow;
}

void main(){
    
    float lambert = 0.0;
    /*
    vec3 v_lightDir = normalize(v_lightPosition - v_positionVarying.xyz);
    lambert = max(dot(v_lightDir, v_normalVarying), 0.0);
    
    fragColor = vec4(vec3(lambert * ShadowCalculation(v_shadowCoord)), 1.0);
    */
    
    
    float isPerspective = 1.0;
    if(isPerspective==1.0) {
        vec3 v_lightDir = normalize(v_lightPosition - v_positionVarying.xyz);
        lambert = max(dot(v_lightDir, v_normalVarying), 0.0);
    } else {
        //lambert = max(dot(normalize(vLightPosition), vNormalVarying), 0.0);
    }
    float shadowVal = ShadowCalc(v_shadowCoord);
    fragColor = vec4(vec3(lambert * shadowVal), 1.0);

}