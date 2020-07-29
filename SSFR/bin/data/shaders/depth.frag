
#version 150
precision highp float;

in vec3 vpos;
in vec2 vTexCoord;
out vec4 fragColor;

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
    uv = uv * 2.0 - vec2(1.0 ,1.0);
    
    normal.xy = uv;
    float radius_sq = dot(uv.xy, uv.xy);
	if (radius_sq > 1.0) {
        discard;
    }
    normal.z = sqrt(1.0 - radius_sq);
    normal.z = normal.z * 2.0 - 1.0;
    vec4 viewPos = vec4(vpos.xyz + normalize(normal) * size,  1.0);
	vec4 screenSpacePos =  projectionMatrix * viewPos;
    float depth = screenSpacePos.z / screenSpacePos.w;



    fragColor = vec4(vec3(depth), 1.0);
}