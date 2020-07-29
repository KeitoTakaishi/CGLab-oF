#version 150
#extension GL_ARB_texture_rectangle : enable
//#extension GL_EXT_gpu_shader4 : require
precision highp float;

in vec3 pos;
//attribute vec3 pos;
uniform mat4 modelViewProjectionMatrix;
out float v_discard;
//varying float v_discard;

//uniform mat4 u_mvpMatrix;
//uniform mat4 u_normalMatrix;
//uniform sampler2DRect tex; // triTableの値を格納するテクスチャ
uniform sampler2DRect tex; // triTableの値を格納するテクスチャ
uniform vec3 u_cellNum; // セル数
uniform vec3 u_cellSize; // セルの大きさ
uniform float u_time;

// 符号付き距離関数を返す関数
// サンプルで実際に使用しているものを載せるのは冗長なため、ここでは単純な球の距離関数を使用している
float getDistance(vec3 p) {
  return length(p) - 2.0;
}

// 法線を返す関数
vec3 getNormal(vec3 p) {
  float e = 0.01;
  return normalize(vec3(
    getDistance(p + vec3(e, 0.0, 0.0)) - getDistance(p - vec3(e, 0.0, 0.0)),
    getDistance(p + vec3(0.0, e, 0.0)) - getDistance(p - vec3(0.0, e, 0.0)),
    getDistance(p + vec3(0.0, 0.0, e)) - getDistance(p - vec3(0.0, 0.0, e))
  ));
}

// v0, v1の値をもとにp0, p1を補間した値を返す
vec3 interpolate(vec3 p0, vec3 p1, float v0, float v1) {
  return mix(p0, p1, -v0 / (v1 - v0));
}

void main(void) {
  int cellId = gl_VertexID / 15; // セルのID
  float vertexId = mod(gl_VertexID , 15); // セル内での頂点のID
  
  float x = mod(cellId, u_cellNum.x);
  float y = mod(cellId , (u_cellNum.x * u_cellNum.y)) / u_cellNum.x;
  float z = cellId / (u_cellNum.x * u_cellNum.y);
  vec3 cellIdx = vec3(x, y, z);
  cellIdx = floor(cellIdx);
  vec3 cellCorner = (0.5 * vec3(u_cellNum) - vec3(cellIdx)) * u_cellSize;
  //debugColor = normalize(cellCorner);


  //vec2 xy = vec2(mod(gl_VertexID, 100) -50.0, gl_VertexID/100.0-50);
  //vec2 xy = vec2(mod(gl_VertexID, 50.0),gl_VertexID/50.0 );
  //gl_Position = modelViewProjectionMatrix * vec4(xy.x, xy.y, 0.0, 1.0);
  //gl_PointSize = 100.0; 
  // 現在のセルの各頂点位置を求める
  vec3 c0 = cellCorner;
  vec3 c1 = cellCorner + u_cellSize * vec3(1.0, 0.0, 0.0);
  vec3 c2 = cellCorner + u_cellSize * vec3(1.0, 1.0, 0.0);
  vec3 c3 = cellCorner + u_cellSize * vec3(0.0, 1.0, 0.0);
  vec3 c4 = cellCorner + u_cellSize * vec3(0.0, 0.0, 1.0);
  vec3 c5 = cellCorner + u_cellSize * vec3(1.0, 0.0, 1.0);
  vec3 c6 = cellCorner + u_cellSize * vec3(1.0, 1.0, 1.0);
  vec3 c7 = cellCorner + u_cellSize * vec3(0.0, 1.0, 1.0);

  // 現在のセルの各頂点の値を求める
  float v0 = getDistance(c0);
  float v1 = getDistance(c1);
  float v2 = getDistance(c2);
  float v3 = getDistance(c3);
  float v4 = getDistance(c4);
  float v5 = getDistance(c5);
  float v6 = getDistance(c6);
  float v7 = getDistance(c7);
  
 
  // セルの各頂点の値からルックアップテーブルを参照するためのインデックスを求める
  int cubeIdx = 0;
  if (v0 < 0.0) cubeIdx |= 1;
  if (v1 < 0.0) cubeIdx |= 2;
  if (v2 < 0.0) cubeIdx |= 4;
  if (v3 < 0.0) cubeIdx |= 8;
  if (v4 < 0.0) cubeIdx |= 16;
  if (v5 < 0.0) cubeIdx |= 32;
  if (v6 < 0.0) cubeIdx |= 64;
  if (v7 < 0.0) cubeIdx |= 128;
  
  
  //ivec2 iuv = ivec2 ((cubeIdx*16.0+vertexId), 0);
  //ivec2 iuv = ivec2(0, 0);
  //float tri = texelFetch(tex, iuv).r;
  //float tri = texelFetch(tex, ivec2(0, 0)).r;

  
  //float delta =  1.0 / 4096.0;
  //delta *= 0.5;
  //float _x = 17.0 / 4096.0;

  float res = 8.0;
  float delta = 0.5;
  float _x = cubeIdx*16.0+vertexId;
  v_discard = 0.0;
  //float tri = texture2DRect(tex, vec2(2.5, 0.0)).r;
  float tri = texture2DRect(tex, vec2(x+delta, 0.5)).r;
  //float tri = texture2DRect(tex, vec2(0.5, 0.5)).r;
  v_discard = 0.0;
    if (tri < 0.0) {
      //gl_Position = vec4(vec3(0.0), 1.0);
      v_discard = 0.0;
    }
    else {
    // 描画する三角形があるときは、頂点位置を求める
    vec3 position = vec3(0.0, 0.0, 0.0);
    if (tri == -1.0) {
      position = interpolate(c0, c1, v0, v1);
    } 
    else if (tri == 1.0) {
      position = interpolate(c1, c2, v1, v2);
    } 
    else if (tri == 2.0) {
      position = interpolate(c2, c3, v2, v3);
    } else if (tri == 3.0) {
      position = interpolate(c3, c0, v3, v0);
    } else if (tri == 4.0) {
      position = interpolate(c4, c5, v4, v5);
    } else if (tri == 5.0) {
      position = interpolate(c5, c6, v5, v6);
    } else if (tri == 6.0) {
      position = interpolate(c6, c7, v6, v7);
    } else if (tri == 7.0) {
      position = interpolate(c7, c4, v7, v4);
    } else if (tri == 8.0) {
      position = interpolate(c0, c4, v0, v4);
    } else if (tri == 9.0) {
      position = interpolate(c1, c5, v1, v5);
    } else if (tri == 10.0) {
      position = interpolate(c2, c6, v2, v6);
    } else if (tri == 11.0) {
      position = interpolate(c3, c7, v3, v7);
    }
    gl_Position = modelViewProjectionMatrix * vec4(position, 1.0);
    v_discard = 0;
  }
  
}