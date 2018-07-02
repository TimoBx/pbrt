// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2015-2016 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#version 330

layout(location = 0) out vec4 outbuff;

uniform sampler2D distSrc;
uniform int       numPass;

in vec4 uv[5];

void main(void) {
  vec4 dataD;
  vec2 pos   = uv[0].xy;
  vec2 np    = pos;
  vec4 dataO = texture(distSrc,pos);
  float dist = dataO.b;
  float tmp  = 0.;

  for(int i=1;i<5;i++) {
    dataD   = texture(distSrc,uv[i].xy);
    tmp      = dataD.b +  abs(uv[i].x-pos.x) + abs(uv[i].y-pos.y);
    
    if(tmp<dist) {
      np = dataD.xy;//uv[i].xy;
      dist = tmp;
    }

    dataD   = texture(distSrc,uv[i].zw);
    tmp      = dataD.b +  abs(uv[i].z-pos.x) + abs(uv[i].w-pos.y);

    if(tmp<dist) {
      np = dataD.xy;//uv[i].zw;
      dist = tmp;
    }
  }
  
  //outbuff = numPass==0 ? vec4(pos,vec2(dist,1.)) : vec4(dataO.rg,vec2(dist,1.));
  outbuff = numPass==0 ? vec4(pos,vec2(dist,1.)) : vec4(np,vec2(dist,1.));
}
