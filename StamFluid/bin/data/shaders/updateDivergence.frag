#version 150
#extension GL_ARB_explicit_attrib_location : enable
precision mediump float;

uniform sampler2DRect vel;
uniform sampler2DRect div;
uniform sampler2DRect press;

in vec2 vTexCoord;

//layout (location = 0) out vec4 vFragColor0;
//layout (location = 1) out vec4 vFragColor1;
//layout (location = 2) out vec4 vFragColor2;

out vec4 _div;

void main(){
    vec2 uv = vTexCoord.xy;
    //uv += vec2(0.5, 0.5);
    float x0 = texture(vel, uv - vec2(1.0, 0.0)).x;//right
    float x1 = texture(vel, uv + vec2(1.0, 0.0)).x;//left
    float y0 = texture(vel, uv - vec2(0.0, 1.0)).y;//down
    float y1 = texture(vel, uv + vec2(0.0, 1.0)).y;//up

    float divergence = 0.5*((x1 - x0 + y1 - y0));
    
    // vFragColor0 = vec4 (texture(vel, uv).rgb, 1.0);
    // vFragColor1 = vec4(divergence, divergence, 0.0, 1.0);    
    // vFragColor2 = vec4(0.0, 0.0, 0.0, 1.0);    
    _div = vec4(divergence, divergence, 0.0, 1.0);
}