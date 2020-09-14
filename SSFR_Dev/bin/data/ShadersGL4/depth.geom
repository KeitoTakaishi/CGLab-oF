#version 400
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;
//layout (points, max_vertices = 1) out;


uniform mat4 modelViewProjectionMatrix;

/*
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 invViewMatrix;
uniform mat4 invProjectionMatrix;
*/


uniform mat4 v;
uniform mat4 p;
uniform mat4 iv;
uniform mat4 ip;
uniform float size;

out vec2 vTexCoord;
out vec3 vpos;

vec3[4] g_pos = vec3[](
    vec3(-1, 1, 0),
	vec3( 1, 1, 0),
	vec3(-1,-1, 0),
	vec3( 1,-1, 0)
    /*
    vec3(-1, -1, 0),
    vec3(1, -1, 0),
    vec3(1, 1, 0),
    vec3(-1, 1, 0)
    */
);

vec2[4] g_tex = vec2[](
		vec2(0, 1),
		vec2(1, 1),
		vec2(0, 0),
		vec2(1, 0)
);

/*
modelg等列にはどうやら意味があるらしい
//vec4 clip = modelViewProjectionMatrix * gl_in[0].gl_Position;
//vec4 clip = modelViewProjectionMatrix * gl_in[0].gl_Position;
*/
void main(){
    for(int i = 0; i < 4; i++){
        /*
        vec3 pos = g_pos[i] * size;
        vec3 wpos = (invViewMatrix * vec4(pos, 1.0) + gl_in[0].gl_Position).xyz;
        gl_Position =  projectionMatrix * viewMatrix * vec4(wpos, 1.0);
        vTexCoord = g_tex[i];
        vpos = (viewMatrix * vec4(wpos,1.0)).xyz;
        */
        
        vec4 clip = modelViewProjectionMatrix * gl_in[0].gl_Position;
        vec4 particleCenterPosInWorld = (iv * (ip * clip));
       
        //g_posはカメラから見て(1, 1), (-1, 1)とかなので
        //invViewMatrixの変換でワールド座標へ変換される
        vec4 wPos = (iv * vec4(g_pos[i].xyz * size, 0.0)) + particleCenterPosInWorld;
        gl_Position = p * v * vec4(wPos.xyz, 1.0);
        //gl_Position = clip;
        vpos = (v * vec4(wPos.xyz, 1.0)).xyz;
        vTexCoord = g_tex[i];
        
        EmitVertex();
    }

    EmitVertex();
    EndPrimitive();
}