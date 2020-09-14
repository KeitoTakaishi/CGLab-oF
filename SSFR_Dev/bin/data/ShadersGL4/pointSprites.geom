#version 400
//#extension GL_EXT_geometry_shader4 : enable
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

//uniform mat4 modelViewProjectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(){
    
    for(int i = 0; i < 4; i++){
        //vec4 clip = modelViewProjectionMatrix * gl_in[0].gl_Position;
        //gl_Position = projectionMatrix * viewMatrix * vec4(wPos.xyz, 1.0);
        gl_Position = gl_in[0].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}