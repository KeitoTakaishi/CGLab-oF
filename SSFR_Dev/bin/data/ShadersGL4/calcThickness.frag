#version 400
precision highp float;

in vec3 vpos;
in vec2 vTexCoord;
out vec4 fragColor;

uniform float size;
uniform mat4 p;
uniform vec2 camClips;

float LinearizeDepth(float depth)  
{  
    //vec2 camClips = vec2(0.01, 500.0);
	return (2.0 * camClips.x) / (camClips.y + camClips.x - depth * (camClips.y - camClips.x));  
} 

float Linear01Depth( float z )
{
    //x は (1-far/near)、 y は (far/near)
    //float near = camClips.x;
    float near = 0.1 ;
    float far = 100.0;
    return 1.0 / ((1.0 - far/near) * z + (far/near));
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
	//vec4 screenSpaceFrontPos =  p * viewFrontPos;
	vec4 screenSpaceFrontPos =  viewFrontPos;
    float frontDepth = screenSpaceFrontPos.z;//表までのdepth
    
    //frontDepth = (frontDepth + 1.0) * 0.5;
    //frontDepth = Linear01Depth(frontDepth);
    

    
    //normal.z = -1.0 * normal.z;
    normal.z = (-1.0 + radius_sq);
    vec4 viewBackPos = vec4(vpos.xyz + normalize(normal) * size,  1.0);
	//vec4 screenSpaceBackPos =  p * viewBackPos;
	vec4 screenSpaceBackPos =  viewBackPos;
    //depth = Linear01Depth(screenSpaceBackPos.z / screenSpaceBackPos.w) - depth;//裏までのdepth
    float backDepth = screenSpaceBackPos.z ;
    //backDepth = (backDepth + 1.0) * 0.5;
    //backDepth = Linear01Depth(backDepth);
    
    float thickness = abs(backDepth - frontDepth);//裏までのdepth
    
    //depth = (depth * 1.0) * 0.5;
    
    fragColor = vec4(vec3(thickness), 1.0);
}
