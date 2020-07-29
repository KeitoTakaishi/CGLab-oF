#version 150
uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;


//uniform mat4 invCamMatrix;
uniform mat4 viewMatrix;
//uniform mat4 projMatrix;
//uniform mat4 biasMatrix;

uniform mat4 lightSpaceMatrix;
uniform vec3 lightPosition;

in vec4  position;
in vec4  color;
in vec3  normal;
in vec2  texcoord;


out vec4 colorVarying;
out vec2 texCoordVarying;
out vec3 normalVarying;
out vec4 positionVarying;

out vec3 v_normalVarying;
out vec4 v_positionVarying;
out vec4 v_shadowCoord;
out vec3 v_lightPosition;

void main(){
    //local
    normalVarying = normal;
    positionVarying = position;

    //view space
    mat3 normalMatrix = transpose(inverse(mat3(modelViewMatrix)));
    v_normalVarying = normalize(vec3(normalMatrix * normal));
    v_positionVarying = modelViewMatrix * position;
    v_lightPosition = vec3(viewMatrix * vec4(lightPosition, 1.0));


    //mainCameraでレンダリングするピクセルからlightまでの距離
    //v_shadowCoord lightからみたクリップ座標系
    v_shadowCoord = lightSpaceMatrix * v_positionVarying;
    //v_shadowCoord = biasMatrix * projMatrix * viewMatrix * invCamMatrix * v_positionVarying;

    
    gl_Position = modelViewProjectionMatrix * position;
}
