#version 150
precision highp float;
//out vec4 outputColor;
/*
void main()
{
    vec4 col = vec4(1.0, 1.0, 0.0, 1.0);
	//outputColor = col;
    gl_FragColor = col;
}
*/
in float v_discard;
//varying float v_discard;
//in vec3 debugColor;

//uniform sampler2DRect tex;
//uniform sampler2D tex;
out vec4 oC;

const vec3 LIGHT_DIR = normalize(vec3(0.5, 0.5, 1.0));

void main(void) {
    
  /*
  float c  = texture2D(tex, vec2(0.0, 0.0)).r;
  if (c == -1.0) {
    oC = vec4(1.0, 0.0, 1.0, 1.0);
  }else{
    oC = vec4(0.0, 0.0, 0.0, 1.0);
  }
  */
  
  
  if (v_discard == 1.0) {
    oC = vec4(0.0, 1.0, 0.0, 1.0);
    //discard;
    //gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);

  } 
  else {
    //vec3 normal = normalize(vec3(0.0, 0.0, 0.0));
    //vec3 color = vec3(0.3, 0.3, 1.0) * max(0.0, dot(LIGHT_DIR, normal));
    //vec3 color = vec3(1.0, 1.0, 1.0);
    //gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    oC = vec4(1.0, 1.0, 1.0, 1.0);
    //discard;
  }
  

  /*
  float delta = 1.0 / 16.0;
  //float c  = texture2D(tex, vec2(7.0/8.0+delta, delta)).r;
  float c  = texelFetch(tex, ivec2(6.0, 0)).r;

  vec4 col = vec4(0.0, 0.0, 0.0, 1.0);
  if(c == 0.0){
    col.rgb = vec3(1.0, 0.0, 0.0);
  }else if(c == 6.0){
    col.rgb = vec3(1.0, 0.0, 1.0);
  }
  oC = col;
  */
  
  //oC = vec4(1.0, 0.0, 0.0, 1.0);
  //gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}