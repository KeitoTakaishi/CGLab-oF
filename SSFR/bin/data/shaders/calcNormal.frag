#version 150
precision highp float;
uniform sampler2DRect depthTex;
uniform mat4 viewMatrix;
uniform mat4 invProjectionMatrix;
uniform vec2 screenScale;


in vec2 vTexCoord;
out vec4 fragOut;


vec3 getViewPos(vec2 texcoord){
    float depth = texture(depthTex, texcoord).r;
    //正規化
    vec2 uv = texcoord / screenScale;
    //-1.0 ~ 1.0
    uv = uv * 2.0 - vec2(1.0, 1.0);
    vec4 clipPos = vec4(uv, depth *2.0, 1.0);
    vec4 viewPos = invProjectionMatrix * clipPos;
    //vec4 viewPos =  clipPos * invProjectionMatrix;
    viewPos.xyz /= viewPos.w;
    return viewPos.xyz;
}

void main(){
    float depth = texture(depthTex, vTexCoord).r;
    
    if(depth > 1.0 || depth < 0.0001) discard;
    vec3 viewPos = getViewPos(vTexCoord);
    
    
    vec3 ddx =  getViewPos(vTexCoord + vec2(1.0, 0.0)) - viewPos;
    vec3 ddx2 = viewPos - getViewPos(vTexCoord + vec2(1.0, 0.0));
    if(abs(ddx.z) > abs(ddx2.z)){
        ddx = ddx2;
    }
    vec3 ddy =  getViewPos(vTexCoord + vec2(0.0, 1.0)) - viewPos;
    vec3 ddy2 =  viewPos - getViewPos(vTexCoord + vec2(0.0, 1.0));
     if(abs(ddy.z) > abs(ddy2.z)){
        ddy = ddy2;
    }
    vec3 N = normalize(cross(ddx, ddy));
    
    //Todo
    //N.y *= -1.0;
    N = N * 0.5 + vec3(0.5, 0.5, 0.5);
    fragOut = vec4(N, 1.0);
}