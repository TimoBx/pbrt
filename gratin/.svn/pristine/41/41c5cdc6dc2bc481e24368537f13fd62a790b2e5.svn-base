// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2015-2016 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#version 330

layout(location = 0) in vec2 vertex;
layout(location = 1) in vec2 coord;

uniform sampler2D distSrc;
uniform int       numPass;

out vec4 uv[5];

void main() {	
  vec2 pasO = pow(2.,float(numPass))/vec2(textureSize(distSrc,0));

  uv[0].xy = coord; uv[0].wz = pasO;
  uv[1].xy = coord - vec2(    0., pasO.y);  uv[1].zw = coord + vec2(    0., pasO.y);
  uv[2].xy = coord - vec2(pasO.x,     0.);  uv[2].zw = coord + vec2(pasO.x,     0.);
  uv[3].xy = coord - vec2(pasO.x,-pasO.y);  uv[3].zw = coord + vec2(pasO.x,-pasO.y);
  uv[4].xy = coord - vec2(pasO.x, pasO.y);  uv[4].zw = coord + vec2(pasO.x, pasO.y);
  
  gl_Position = vec4(vertex,0,1);
}
