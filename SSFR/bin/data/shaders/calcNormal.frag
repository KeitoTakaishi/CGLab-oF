#version 150
precision highp float;
uniform sampler2DRect depthTex;
uniform vec2 screenScale;
uniform mat4 viewMatrix;
uniform mat4 invProjectionMatrix;

in vec2 vTexCoord;
out vec4 fragOut;


vec3 getViewPos(vec2 texcoord){
    float depth = texture(depthTex, texcoord).r;
    vec2 uv = texcoord / screenScale;
    uv = uv * 2.0 - vec2(1.0, 1.0);
    vec4 clipPos = vec4(uv, depth , 1.0);
    vec4 viewPos = invProjectionMatrix * clipPos;
    viewPos.xyz /= viewPos.w;
    return viewPos.xyz;
}

void main(){
    vec3 viewPos = getViewPos(vTexCoord);
    float depth = texture(depthTex, vTexCoord).r;
    if(depth > 1.0 || depth < 0.0001) discard;
    vec3 ddx =  getViewPos(vTexCoord + vec2(1.0, 0.0)) - viewPos;
    vec3 ddy =  getViewPos(vTexCoord + vec2(0.0, 1.0)) - viewPos;
    vec3 N = normalize(cross(ddx, ddy));
    N = normalize(viewMatrix * vec4(N, 0.0)).xyz;
    fragOut = vec4(N*0.5 + 0.5, 1.0);
    //fragOut = vec4(vTexCoord/screenScale, 0.0, 1.0);
}