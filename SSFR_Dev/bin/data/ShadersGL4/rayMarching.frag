#version 400

out vec4 outputColor;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 view;


uniform vec2 resolution;
uniform float fov;
uniform vec3 camUp;
uniform vec3 camPosition;
uniform vec3 lightPosition;
//CubeMap
//uniform float eta;
//uniform float refrectionFactor;
uniform samplerCube cubuMapTex;


#define PI 3.14159265359
#define DEG2RAD (PI/180.0)

const float sphereSize = 1.5;
const vec3 lightDir = vec3(-0.577, 0.577, 0.577);

float distanceFunc(vec3 p){
    return length(p) - sphereSize;
}

vec3 getNormal(vec3 p){
    float d = 0.0001;
    return normalize(vec3(
                          distanceFunc(p + vec3(  d, 0.0, 0.0)) - distanceFunc(p + vec3( -d, 0.0, 0.0)),
                          distanceFunc(p + vec3(0.0,   d, 0.0)) - distanceFunc(p + vec3(0.0,  -d, 0.0)),
                          distanceFunc(p + vec3(0.0, 0.0,   d)) - distanceFunc(p + vec3(0.0, 0.0,  -d))
                          ));
}

float compute_depth(vec4 clippos)
{
    //return ((clippos.z / clippos.w) + 1.0) * 0.5;
    return (clippos.z / clippos.w) ;
}

void main()
{
    vec2 p = (gl_FragCoord.xy * 2.0 - resolution) / min(resolution.x, resolution.y);
    
    // camera
    vec3 cPos = camPosition;
    vec3 cDir = -normalize(camPosition);
    vec3 cUp = camUp;
    vec3 cSide = cross(cDir, cUp);
    float targetDepth = 1.0 / tan(fov * 0.35 * DEG2RAD);
    
    // ray
    vec3 ray = normalize(cSide * p.x + cUp * p.y + cDir * targetDepth);
    
    // marching loop
    float dis = 0.0; // レイとオブジェクト間の最短距離
    float rLen = 0.0;     // レイに継ぎ足す長さ
    vec3  rPos = cPos;    // レイの先端位置
    for(int i = 0; i < 16; i++){
        dis = distanceFunc(rPos);
        rLen += dis;
        rPos = cPos + ray * rLen;
    }
    
    // hit
    vec4 wPos =  view * vec4(rPos, 1.0);
    vec3 wNormal = getNormal(rPos);//world normal
    vec3 wView = camPosition - rPos.xyz;
    vec3 refDir = reflect(-wView, wNormal);
    float eta = 0.1;
    vec3 refrDir = refract(-wView, wNormal, eta);

    if(abs(dis) < 0.001){

        //debug
        refDir.y = -1.0 * refDir.y;
        refrDir.y *= -1.0; 
        vec3 reflectCol = texture(cubuMapTex, normalize(refDir)).rgb;
        vec3 refractCol = texture(cubuMapTex, normalize(refrDir)).rgb;

        float refrectionFactor = 0.1;
        vec4 c = vec4( mix(reflectCol, refractCol, refrectionFactor), 0.75);

        //diffuse + specular
        vec3 lightPos = vec3(lightPosition.x, lightPosition.y * -1.0, lightPosition.z);
        vec3 lightPosView = (view * vec4(lightPos, 1.0)).xyz;
        vec3 s = normalize(lightPosView - wPos.xyz);
        vec3 n = normalize(wNormal);
        float diffuse = max(dot(n, s), 0.0);
        diffuse = diffuse * 0.5 + 0.5;
        vec3 r = reflect(-s, n);
        vec3 v = normalize(-vec3(wPos));
        float spec = pow( max(dot(r, v), 0.0), 10.0) * 2.;

        vec4 finalColor = c * diffuse + spec * vec4(1.0, 1.0, 1.0, 0.0);
        outputColor = vec4(finalColor.rgb, 0.35);
        //outputColor = vec4(1.0, 0.0, 0.0, 0.8);
      
        //debug
        vec4 viewNormal = view * vec4(wNormal, 0.0);
        float d = compute_depth((modelViewProjectionMatrix * vec4(rPos, 1.0)));
        gl_FragDepth = d;

    }else{
        outputColor = vec4(vec3(0.0, 0.0, 0.0), 1.0);
        gl_FragDepth = 1.0;
    }

    //gNormal = vec4(1.0, 0.0, 0.0, 1.0);
}
