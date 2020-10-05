#version 400
uniform samplerCube	EnvMap;
uniform vec3 cameraPos;
uniform int reflection;
in vec3 vPosition;
in vec2 vTexCoord;
in vec3 vNormal;
in vec3 envBoxvNormal;

out vec4 fragOut;

void main(){
    //vec3 ref = reflect(vPosition - cameraPos, vNormal);
    vec3 ref = vec3(0.0, 0.0, 0.0);
    if(reflection == 0){
        ref = reflect(vPosition - cameraPos, vNormal);
    }else{
        //ref = vNormal;
        ref = envBoxvNormal;
    }
    vec4 envColor  = texture(EnvMap, normalize(ref));
    //fragOut = vec4(texture(EnvMap, normalize(vNormal).rgb, 1.0));
    fragOut = envColor;
    //fragOut = vec4(1.0, 0.0, 0.0, 1.0);
}
