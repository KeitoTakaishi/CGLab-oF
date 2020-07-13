#version 150
uniform float geomMode;
uniform vec2 camPrames;
in float depth;
out vec4 outputColor;


float LinearizeDepth(float depth)  
{  
    vec2 camClips = vec2(camPrames.x, camPrames.y);
	return (2.0 * camClips.x) / (camClips.y + camClips.x - depth * (camClips.y - camClips.x));  
} 

void main()
{
   //outputColor = texture(tex, texCoordVarying * size);
    outputColor = vec4(vec3(LinearizeDepth(depth)), 1.0);
}