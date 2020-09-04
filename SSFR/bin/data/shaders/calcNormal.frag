#version 400
precision highp float;
uniform sampler2DRect depthTex;
uniform mat4 viewMatrix;
uniform mat4 invProjectionMatrix;
uniform vec2 screenScale;
uniform vec2 camClips;

in vec2 vTexCoord;
out vec4 fragOut;

float LinearizeDepth(float depth)  
{  
	return (2.0 * camClips.x) / (camClips.y + camClips.x - depth * (camClips.y - camClips.x));  
} 


float Linear01Depth( float z )
{
    //x は (1-far/near)、 y は (far/near)
    float near = camClips.x;
    float far = camClips.y;
    return 1.0 / ((1.0 - far/near) * z + (far/near));
}

vec3 getViewPos(vec2 texcoord){
    float depth = texture(depthTex, texcoord).r;
    depth =  Linear01Depth(depth);
    //正規化
    vec2 uv = texcoord / screenScale;
    //-1.0 ~ 1.0
    uv = uv * 2.0 - vec2(1.0, 1.0);
    
    
    vec4 clipPos = vec4(uv.x, uv.y, (depth), 1.0);
    vec4 viewPos = invProjectionMatrix * clipPos;
    viewPos.xyz /= viewPos.w;
    return viewPos.xyz;  
}


void main(){
    //vTexCoord : vec2(0.0, 0.0) ~ vec2(screeScale)
    vec2 uv = vTexCoord;
    float depth = texture(depthTex, uv).r;
    //depth = Linear01Depth(depth);
    if(depth > 1.0 || depth < 0.0001) discard;
    //Coord from camera


    vec3 viewPos = getViewPos(uv);
    
    
    float ep = 1.0;
    vec3 ddx =  getViewPos(uv + vec2(ep, 0.0)) - viewPos;
    //vec3 ddx =  getViewPos(uv, vec2(ep, 0.0)) - viewPos;
    
    vec3 ddx2 = viewPos - getViewPos(vTexCoord + vec2(-1.0, 0.0));
    if(abs(ddx.z) > abs(ddx2.z)){
         //ddx = ddx2;
    }
    
    
    vec3 ddy =  getViewPos(uv + vec2(0.0, ep)) - viewPos;
    //vec3 ddy =  getViewPos(uv, vec2(0.0, ep)) - viewPos;
    
    vec3 ddy2 =  viewPos - getViewPos(vTexCoord + vec2(0.0, -1.0));
    if(abs(ddy.z) > abs(ddy2.z)){
        //ddy = ddy2;
    }
    
    
    vec3 N = normalize(cross(ddx, ddy));
    N = normalize( viewMatrix *  vec4(N, 0.0)).xyz; 

    
    //Todo
    //N.y = 1.0 - N.y;
    N = N * 0.5 + vec3(0.5, 0.5, 0.5);
    fragOut = vec4(vec3( N  ), 1.0);
}