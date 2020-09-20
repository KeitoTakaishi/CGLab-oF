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
uniform mat4 invViewMatrix;


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
    vec3 normal = texture(normalTex, vTexCoord).xyz;//view空間でのnormalベクトル
    //normal.y *= -1.0;
    
    
    //normal = normalize(normal);
    
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
    

    //frenel
    float F0 = 0.15;
    float frenel = F0 + (1.0 - F0) * pow(1.0- dot(v, n), 5.0);
    
    //Absorb
    //float _d = clamp(thickness, 0.0, 50.0);
    
    thickness *= 1.;
    vec3 absorbColor = vec3(exp(-absorbK.x * thickness), exp(-absorbK.y * thickness), exp(-absorbK.z * thickness));
    absorbColor*= 1.0;
    

    //CubeMap
    mat4 iview = inverse(transpose(inverse(viewMatrix)));
    vec3 worldNormal = normalize(iview * vec4(normal, .0)).xyz;
    vec4 worldPos = invViewMatrix * vec4(position, 1.0);
    //reflection----------
    vec3 refl = reflect(normalize(worldPos.xyz - camPos), normalize(worldNormal));
    vec4 reflColor  = texture(cubuMapTex, refl);

    
    //refraction----------
    vec3 refr = refract(normalize(worldPos.xyz - camPos), normalize(worldNormal), 1.333);
    vec4 refrColor  = texture(cubuMapTex, normalize(refr));

    vec3 diffuseCol = vec3(0.0, 0.8, 1.0) * diffuse;
    vec3 specCol = vec3(1.0, 1.0, 1.0) * spec;
    vec3 frenelCol = reflColor.rgb * frenel;

    vec3 lightingColor = diffuseCol + specCol + frenelCol + absorbColor;
    //vec3 lightingColor = specCol + frenelCol + absorbColor;
    //vec3 lightingColor = frenel*envColor.rgb;
    //vec3 lightingColor = envColor.rgb;

    //vec4 finalOutColor = mix(refrColor, vec4(lightingColor, 1.0), thickness* 0.8);
    //vec4 finalOutColor = mix(refrColor, vec4(lightingColor, 1.0), clamp(thickness, 0.0, 0.65));
    //vec4 finalOutColor = vec4(lightingColor, clamp(thickness, 0.0, 0.85) );
    
    vec4 finalOutColor = vec4(lightingColor, clamp(thickness, 0.0, 0.85));
    fragOut = finalOutColor; 
    
    

    //worldNormal = normalize(worldNormal);
    //fragOut = vec4(worldNormal.x, worldNormal.y, worldNormal.z, 1.0);
    //fragOut = vec4(position.x * 0.0, position.y * 1.0, position.z*0.0, 1.0);
    
    //fragOut = reflColor; 
    //fragOut = refrColor;    
}