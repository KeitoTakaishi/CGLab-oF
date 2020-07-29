#version 150
precision highp float;

in vec3 vpos;
in vec2 vTexCoord;
out vec4 fragColor;

uniform float size;
uniform mat4 projectionMatrix;
uniform vec2 camClips;

float LinearizeDepth(float depth)  
{  
    //vec2 camClips = vec2(0.01, 500.0);
	return (2.0 * camClips.x) / (camClips.y + camClips.x - depth * (camClips.y - camClips.x));  
} 


void main() {
    vec3 normal = vec3(0.0, 0.0, 0.0);

    vec2 uv = vTexCoord;
    uv = uv * 2.0 - vec2(1.0 ,1.0);
    normal.xy = uv.xy;
    

    float radius_sq = dot(uv.xy, uv.xy);
	
    if (radius_sq > 1.0) {
        discard;
    }

    normal.z = (1.0 - radius_sq);
   


    vec4 viewFrontPos = vec4(vpos.xyz + normalize(normal) * size,  1.0);
	vec4 screenSpaceFrontPos =  projectionMatrix * viewFrontPos;
    float depth = screenSpaceFrontPos.z / screenSpaceFrontPos.w;//表までのdepth
    depth = LinearizeDepth(depth);

    vec4 viewBackPos = vec4(vpos.xyz - normalize(normal) * size,  1.0);
	vec4 screenSpaceBackPos =  projectionMatrix * viewBackPos;
    depth = LinearizeDepth(screenSpaceBackPos.z / screenSpaceBackPos.w) - depth;//表までのdepth
    
    fragColor = vec4(vec3(depth ), 1.0);
}
