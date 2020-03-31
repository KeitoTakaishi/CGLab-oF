#version 150

uniform sampler2D tex0;
uniform vec2 camClips;
in vec2 Texcoord;

out vec4 col;

float LinearizeDepth(float zoverw)  
{  
	return (2.0 * camClips.x) / (camClips.y + camClips.x - zoverw * (camClips.y - camClips.x));  
}  

void main(){
    float depth = texture2D(tex0, Texcoord ).r;
    depth = LinearizeDepth(depth);
    col = vec4( vec3(depth ), 1.0);
}