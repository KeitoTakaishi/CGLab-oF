#version 400

uniform float u_time;
uniform vec2 u_resolution;
uniform mat4 view;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat4 projection;
uniform vec3 u_camUp;
uniform float u_far;
uniform float u_near;
uniform float u_radius;

uniform float u_fov;
uniform vec3 u_camPos;

uniform samplerCube cubuMapTex;
//in vec4 v_world;
in vec2 v_texcoord;
in vec4 v_normal;

//layout (location = 0) out vec4 gPosition;
//layout (location = 1) out vec4 gNormal;
out vec4 outputColor;

#define PI 3.14159265359
#define DEG2RAD (PI/180.0)

const float sphereSize = 2.0;
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
    /*
    vec2 p = (gl_FragCoord.xy * 2.0 - u_resolution) / min(u_resolution.x, u_resolution.y);
    p.y = -p.y;
    
    // camera
    vec3 cPos = u_camPos;
    vec3 cDir = -normalize(u_camPos);
    vec3 cUp = u_camUp;
    vec3 cSide = cross(cDir, cUp);
    float targetDepth = 1.0 / tan(70 * 0.5 * DEG2RAD);

    
    vec3 ray = normalize(cSide * p.x + cUp * p.y + cDir * targetDepth);
    
    float distance = 0.0;
    float rLen = 0.0;
    vec3  rPos = cPos;
    for(int i = 0; i < 64; i++){
        distance = distanceFunc(rPos);
        rLen += distance;
        rPos = cPos + ray * rLen;
    }
    vec3 normal = mat3(modelViewMatrix) * getNormal(rPos);   

    gNormal = vec4(0.1, 1.0, 1.0, 1.0);
    if(abs(distance) < 0.01)
    {       
        vec4 world = modelViewMatrix * vec4(rPos, 1.0);
        gPosition = vec4(world.xyz, 1.0);
        gNormal = vec4(normal.rgb, 1.0);
    } else {
        gPosition = vec4(0.0, 0.0, 0.0, 1.0);
        gNormal = vec4(vec3(0.0), 1.0);
    }
    */

     vec2 p = (gl_FragCoord.xy * 2.0 - u_resolution) / min(u_resolution.x, u_resolution.y);
    
    // camera
    vec3 cPos = u_camPos;
    vec3 cDir = -normalize(u_camPos);
    vec3 cUp = u_camUp;
    vec3 cSide = cross(cDir, cUp);
    float targetDepth = 1.0 / tan(70 * 0.5 * DEG2RAD);
    
    // ray
    vec3 ray = normalize(cSide * p.x + cUp * p.y + cDir * targetDepth);
    
    // marching loop
    float distance = 0.0; // レイとオブジェクト間の最短距離
    float rLen = 0.0;     // レイに継ぎ足す長さ
    vec3  rPos = cPos;    // レイの先端位置
    for(int i = 0; i < 16; i++){
        distance = distanceFunc(rPos);
        rLen += distance;
        rPos = cPos + ray * rLen;

    }
    
    // hit
    vec4 wPos = inverse(view) * vec4(rPos, 1.0);
    vec3 normal = getNormal(rPos);
    //normal = normal * 0.5 + 0.5;
    vec3 viewDir = rPos.xyz - u_camPos;
    vec3 refl = reflect(-viewDir, normal);
    vec3 refr = refract(-viewDir, normal, 0.7);

    if(abs(distance) < 0.001){
        vec3 reflectCol = texture(cubuMapTex, normalize(refl)).rgb;
        vec3 refractCol = texture(cubuMapTex, normalize(refr)).rgb;

        //outputColor = vec4( vec3(reflectCol + refractCol)+0.5, 1.);
        //outputColor = vec4(vec3(reflectCol), 1.0);
        outputColor = vec4(vec3(refractCol), 1.0);
        

    }else{
        outputColor = vec4(vec3(0.0, 0.0, 0.0), 1.0);
    }

    //gNormal = vec4(1.0, 0.0, 0.0, 1.0);
}
