// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#version 330

layout(location = 0) out vec4 outbuff;

in vec2 coord;

uniform sampler2D f; 

void main(void) {
 
  vec4 fvals[4];
  vec2 p2 = vec2(1.0)/vec2(textureSize(f,0)*2);
  fvals[0] = texture(f,coord+vec2(-p2.x,-p2.y));
  fvals[1] = texture(f,coord+vec2(-p2.x, p2.y));
  fvals[2] = texture(f,coord+vec2( p2.x, p2.y));
  fvals[3] = texture(f,coord+vec2( p2.x,-p2.y));

  // compute color contraints
  float sum = 0.0;
  float nb  = 0.0;
  for(int i=0;i<4;++i) {
    sum += max(0.0,fvals[i].w);
    nb  += step(0.0,fvals[i].w);
    //sum += fvals[i].w;
    //nb  += step(0.5,fvals[i].z);
  }

  float snb   = sign(nb); 
  outbuff.xyz = fvals[0].xyz+fvals[1].xyz+fvals[2].xyz+fvals[3].xyz;
  outbuff.w   = mix(-1.0,sum/(nb+1.0-snb),snb); 
}
