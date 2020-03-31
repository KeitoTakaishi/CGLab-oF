#version 330

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;

in vec4  position;
in vec4  color;
in vec3  normal;
in vec2  texcoord;

out vec3 v_normalVarying;
out vec4 v_positionVarying;
out vec4 v_shadowCoord;
out vec3 v_lightPosition;

void main() { 
    //v_normalVarying = normal;
    
    //mat3 normalMatrix  = transpose(inverse(mat3(modelViewMatrix)));
    //v_normalVarying = normalize(vec3(normalMatrix * normal));

    mat3 normalMatrix  = inverse(mat3(modelViewMatrix));
    //v_normalVarying = normalize(vec3(normal * normalMatrix));
    v_normalVarying = normalize(vec3( mat3(modelViewMatrix) * normal));
    

    v_positionVarying = modelViewMatrix * position;
    gl_Position = modelViewProjectionMatrix * position;
}
