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

in vec2 texcoord;

uniform sampler2D img;

void main() {
  vec4 c1 = textureOffset(img,texcoord,ivec2(-1,-1));
  vec4 c2 = textureOffset(img,texcoord,ivec2(-1, 1));
  vec4 c3 = textureOffset(img,texcoord,ivec2( 1,-1));
  vec4 c4 = textureOffset(img,texcoord,ivec2( 1, 1));

  outbuff = 0.25*(c1+c2+c3+c4);
}
