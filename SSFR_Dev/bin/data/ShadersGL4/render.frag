#version 400

in vec2 vTexCoord;
in vec3 vNormal;
out vec4 fragOut;


uniform sampler2DRect depthTex;
uniform sampler2DRect positionTex;
uniform sampler2DRect normalTex;
uniform sampler2DRect thicknessTex;
uniform samplerCube cubuMapTex;
uniform mat4 viewMatrix;


uniform int type;
//for Lighting
uniform vec3 lightPos;
uniform float shininess;
uniform vec3 camPos;
uniform vec3 absorbK;

void main(){
    vec2 uv = vTexCoord;
    float depth = texture(depthTex, vTexCoord).x;
    depth = (depth + 1.0) * 0.5;
    if(depth <= 0.5){
        gl_FragDepth = 1.0;
    }else{
        //discard;
        gl_FragDepth = depth;
    }
    
    vec3 position = texture(positionTex, vTexCoord).xyz;
    vec3 normal = texture(normalTex, vTexCoord).xyz;
    float thickness = texture(thicknessTex, vTexCoord).x;

    


    //lighting
    vec3 lightPosView = (viewMatrix * vec4(lightPos, 1.0)).xyz;
    vec3 s = normalize(lightPosView - position);
    vec3 n = normalize(normal);
    float diffuse = max(dot(n, s), 0.0);
    diffuse = diffuse * 0.5 + 0.5;
    diffuse *= 0.5;

    //speclular
    vec3 r = reflect(-s, n);
    vec3 v = normalize(-vec3(position));
    float spec = pow( max(dot(r, v), 0.0), shininess);
    spec *= 0.5;

    //frenel
    float F0 = 0.05;
    float frenel = F0 + (1.0 - F0) * pow(1.0- dot(v, n), 5);
    

    //Absorb
    //float _d = clamp(thickness, 0.0, 50.0);
    thickness *= 5.0;
    vec3 absorbColor = vec3(exp(-absorbK.x * thickness), exp(-absorbK.y * thickness), exp(-absorbK.z * thickness));
    absorbColor*=0.5;
    

    //CubeMap
    vec3 ref = reflect(-normalize(position), normalize(normal));
    vec4 envColor  = texture(cubuMapTex, normalize(ref));

    vec3 diffuseCol = vec3(0.0, 0.8, 0.8) * diffuse;
    vec3 specCol = vec3(1.0) * spec;
    vec3 frenelCol = vec3(1.0) * frenel;

    vec3 lightingColor = diffuseCol + specCol + frenelCol * envColor.rgb + absorbColor;
    vec4 color = vec4(lightingColor, 1.0);
    fragOut = color;   
    //fragOut  = vec4(1.0, 1.0, 1.0, 1.0);
    //fragOut  = vec4(vec3(thickness), 1.0);
    //fragOut = vec4(normal,1.0);   
}