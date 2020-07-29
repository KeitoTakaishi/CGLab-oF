#version 150

uniform sampler2D depth_tex;
//uniform vec2 camClips;
vec2 camClips;
in vec2 Texcoord;

out vec4 col;

float LinearizeDepth(float zoverw)  
{  
    camClips = vec2(100.0, 6000.0);
	return (2.0 * camClips.x) / (camClips.y + camClips.x - zoverw * (camClips.y - camClips.x));  
}  

void main(){
    float depth = texture2D(depth_tex, Texcoord ).r;
    depth = LinearizeDepth(depth);
    col = vec4( vec3(depth ), 1.0);
}