//https://gist.github.com/gyohk/89f274082553c58722cc6b1352a047d7
#version 150

//uniform sampler2DRect depthTex;
uniform sampler2DRect normalTex;
uniform sampler2DRect thicknessTex;
uniform vec3 lightPos;
uniform vec3 camPos;
uniform float alphaCoef;
in vec2 vTexCoord;
out vec4 fragOut;

vec3 lightDir = vec3(0.0, 0.0, 1.0);

void main(){
    vec2 uv = vTexCoord;
    //vec3 normal = texture(depthTex, vTexCoord*vec2(1024.0, 768.0)).xyz;
    //quadにuvを0.0 ~ 1.0で設定しているので定数倍する
    
    vec3 normal = texture(normalTex, vTexCoord*vec2(1024.0, 768.0)).xyz;
    float thickness = texture(thicknessTex, vTexCoord*vec2(1024.0, 768.0)).x;

    //if(normal.x==0.0) discard;
    normal = normal * 2.0 - vec3(1.0, 1.0, 1.0);
    vec3 diffuse = vec3(0.8, 0.8, 0.8);
    //vec3 lightDir = normalize(vec3(0.0, 0.0, 0.0) - lightPos);
    //vec4 c = vec4(vec3(diffuse * dot(normal, lightDir)), 1.0) + vec4(0.0, 0.2, 0.3, 0.0);
    float _diffuse  = clamp(dot(normal, lightDir), 0.0, 1.0);
    //fragOut = vec4(normal.xyz, 1.0);

    vec3 halfV = normalize(lightPos + camPos);
    float specular = pow(clamp(dot(normal, halfV), 0.0, 1.0), 50.0);

    //fragOut = vec4(vec3(vec3(0.0, 0.95, 0.85) * _diffuse + specular), 1.0 );
    fragOut = vec4(vec3(vec3(0.0, 0.95, 0.85) * _diffuse + specular),  thickness * alphaCoef);
    //fragOut = vec4(vec3(thickness ), 1.0);

}