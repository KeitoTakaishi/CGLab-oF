#version 150
precision mediump float;

uniform sampler2DRect vel;
uniform sampler2DRect div;
uniform sampler2DRect press;

uniform float t;
uniform float dt;
uniform vec2 cursor;
uniform vec2 texres;//256.0

in vec2 vTexCoord;

out vec4 vFragColor0;
out vec4 vFragColor1;
out vec4 vFragColor2;


void updateAdvection(){
    vec2 uv = vTexCoord.xy;
    //uv += vec2(0.5, 0.5);
    vec2 v = texture(vel, uv).xy;
    vec2 result = texture(vel, uv - v * dt).xy;
    //vec2 result = texture(vel, uv - texres * v * dt).xy;
    vFragColor0 = vec4(result.x, result.y, 0.0, 1.0);
}

/*
void interactionFore(){
    vec2 uv = vTexCoord.xy;
    vec2 vel = texture(vel, uv).xy;
    float scale = 1.0;
    float dis = dist(cursor, uv);

    float threshold = 0.01;
    if(dis < threshold){
        vel += px * dis;
    }
    //vel書き込む
}
*/

void main() {
    vec2 uv = vTexCoord.xy;
    //float d = distance(uv, cursor/10.0);

    vec2 imguv = uv/texres;
    imguv.y = 1.0 - imguv.y;
    float d = length(cursor - imguv);
    
    vec3 c = vec3(d, 0.0, 0.0);
    //vFragColor0 = vec4(c, 1.0);
    updateAdvection();

    c.rgb = vec3(0.0, d, 0.0);
    vFragColor1 = vec4(c, 1.0);
    
    c.rgb = vec3(0.0, 0.0, d);
    vFragColor2 = vec4(c, 1.0);
}

/*

void updateDivergence(){
    vec2 px = vec2(1.0/texres.x, 1.0/texres.y);
    vec2 uv = vTexCoord.xy/texres + px * 0.5;

    float x0 = texture(vel, uv - vec2(0.5, 0.0)).x;
    float x1 = texture(vel, uv + vec2(0.5, 0.0)).x;
    float y0 = texture(vel, uv - vec2(0.0, 0.5)).y;
    float y1 = texture(vel, uv + vec2(0.0, 0.5)).y;
    float divergence = (x1 - x0 + y1 - y0);

    //divergence書き込み
}

void updatePressure(){
    vec2 px = vec2(1.0/texres.x, 1.0/texres.y);
    vec2 uv = vTexCoord.xy/texres + px * 0.5;

    float x0 = texture(pressure, uv - vec2(1.0, 0.0)).r;
    float x1 = texture(pressure, uv + vec2(1.0, 0.0)).r;
    float y0 = texture(pressure, uv - vec2(0.0, 1.0)).r;
    float y1 = texture(pressure, uv + vec2(0.0, 1.0)).r;

    float d = texture(div, uv);
    float relaxed = (x0 + x1 + y0 + y1 - d) * 0.25;
    //pressure書き込み
    
    // float d = _ResultDivergence[id].r;
    // float relaxed = (x0 + x1 + y0 + y1 - d) * 0.25;
    // _ResultPressure[id] = float4(relaxed.xx, 0.0, 1.0);
    
}

void updateVelocity(){
    vec2 uv = vTexCoord.xy;
    float x0 = texture(press, uv - vec2(1.0, 0.0)).r;
    float x1 = texture(press, uv + vec2(1.0, 0.0)).r;
    float y0 = texture(press, uv - vec2(0.0, 1.0)).r;
    float y1 = texture(press, uv + vec2(0.0, 1.0)).r;
    
    vec2 v = texture(vel, uv).xy;
    vec4 v2 = vec4((v - (vec2(x1, y1) - vec2(x0, y0)) * 0.5), 0.0, 1.0);

    float _Attenuation = 0.9;
    v2 *= _Attenuation;

    //v2書き込み

}

void updateTexture(){
    vec2 uv = vTexCoord.xy;
    vec2 vel = texture(vel, uv).xy;
    vec4 col = texture(tex0, uv - vel * dt);

   
    //_ResultTexture[id] = col;
}
*/