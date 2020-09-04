
#version 400
#extension GL_ARB_explicit_attrib_location : enable
precision highp float;

in vec3 vpos;
in vec2 vTexCoord;
layout (location = 0) out vec4 vFragColor0;
layout (location = 1) out vec4 vFragColor1;

uniform float size;
uniform vec2 camClips;
uniform mat4 projectionMatrix;


float LinearizeDepth(float depth)  
{  
	return (2.0 * camClips.x) / (camClips.y + camClips.x - depth * (camClips.y - camClips.x));  
} 


void main() {
    vec3 normal = vec3(0.0, 0.0, 0.0);
    vec2 uv = vTexCoord;
    
    uv = uv * 2.0 - vec2(1.0, 1.0);
    
    normal.xy = uv;
    float radius_sq = dot(uv.xy, uv.xy);
	if (radius_sq > 1.0) {
        discard;
    }
    normal.z = sqrt(1.0 - radius_sq);
    

    //viewPos : Particleの中心座標
    vec4 viewPos = vec4(vpos.xyz + normalize(normal) * size,  1.0);
	vec4 screenSpacePos =  projectionMatrix * viewPos;
    float depth = screenSpacePos.z / screenSpacePos.w;
    
    
    //depth = LinearizeDepth(depth);
    //depth = (depth+1.0)*0.5;
    vFragColor0 = vec4(vec3(depth), 1.0);
    vFragColor1 = vec4(viewPos);
}