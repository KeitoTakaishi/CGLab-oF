#version 330

in vec4 v_positionVarying;
in vec3 v_normalVarying;

out vec4 fragColor;

uniform vec2 camClip;
void main (void) {
    //fragColor = vec4(1.0);
    //fragColor = vec4(vec3(v_normalVarying), 1.0);


    float depth = smoothstep(camClip.x, camClip.y, length(v_positionVarying.xyz));
    fragColor = vec4(vec3(depth), 1.0);
}