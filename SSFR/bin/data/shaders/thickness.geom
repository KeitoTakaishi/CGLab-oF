#version 400
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;
//layout (points, max_vertices = 1) out;

uniform float size;
uniform mat4 modelViewProjectionMatrix;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 invViewMatrix;
uniform mat4 invpProjectionMatrix;


out vec2 vTexCoord;
out vec3 vpos;

vec3[4] g_pos = vec3[](
    vec3(-1, 1, 0),
	vec3( 1, 1, 0),
	vec3(-1,-1, 0),
	vec3( 1,-1, 0)
);

vec2[4] g_tex = vec2[](
		vec2(0, 1),
		vec2(1, 1),
		vec2(0, 0),
		vec2(1, 0)
);

void main(){
    
    for(int i = 0; i < 4; i++){
        //particle -> billboard
        ///vec4 wPos = (invViewMatrix * vec4(g_pos[i], 0.0)) + gl_in[0].gl_Position;
        
        //space in p
        vec4 clip = modelViewProjectionMatrix * gl_in[0].gl_Position;
        //space in w
        vec4 particlePos = (invViewMatrix * (invpProjectionMatrix * clip));


        //g_posはカメラから見て(1, 1), (-1, 1)とかなので
        //invViewMatrixの変換でワールド座標へ変換される
        vec4 wPos = (invViewMatrix * vec4(g_pos[i].xyz * vec3(size, size , size), 0.0)) + particlePos;
        //wPos.w = 1.0;
        gl_Position = projectionMatrix * viewMatrix * wPos;
        vpos = (viewMatrix * wPos).xyz;
        vTexCoord = g_tex[i];
        EmitVertex();
    }

    EmitVertex();
    EndPrimitive();
}