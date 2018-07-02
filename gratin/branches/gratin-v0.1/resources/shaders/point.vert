// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#version 330

layout(location = 0) in vec2 vertex;

uniform mat4  mvp;
uniform float size;

void main() {   
  gl_PointSize = size;
  gl_Position  = mvp*vec4(vertex,0,1);
}