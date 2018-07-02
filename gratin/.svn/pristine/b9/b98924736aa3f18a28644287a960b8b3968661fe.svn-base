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

// input textures 
uniform sampler2D object;
uniform sampler2D image;

// deformation params
uniform float alphaShading;
uniform float betaShading;
uniform float gammaTexture;
uniform vec2  anchorPoint;

// more control params 
uniform vec4  materialColor;
uniform float gammaCorrect;
uniform float imageOrientation;
uniform float imageScale;

// deformation type (for normal shading or bump mapping)
uniform int   deformType;


vec4 evaluate(in vec2 coord,in vec4 currentShape) {
  // evaluate the primitive at the given coordinates (either as normal shading or bump mapping)
  vec4 c = texture(image,coord);

  if(deformType==1) {
    // bump mapping
    vec3 nf = normalize(currentShape.xyz+vec3(c.xy,0.0)*gammaCorrect);
    return vec4(nf,c.w);
  } else {
    // normal shading 
    c = c*materialColor*c.w;
    return vec4(pow(c.xyz,vec3(gammaCorrect)),c.w);
  }
}

vec2 globalDeformation(in vec2 coord) {
  // global rotation + scaling + translation (convenient function to control the input image)
  vec2 d = vec2(cos(imageOrientation),sin(imageOrientation));

  // translate/scale 
  vec2 npos = (coord-anchorPoint)*imageScale;
  
  // rotate/recenter
  return vec2(d.x*npos.x+d.y*npos.y,-d.y*npos.x+d.x*npos.y)+vec2(0.5);
}

float fp(in float u) {
  // first-order deformation magnitude (texture deformation)
  float expg = exp(gammaTexture);
  float tmpg = expg-1.0;
  return (2.0*u*tmpg)/((expg+1.0)-u*tmpg);
}

float fn(in float u) {
  // second-order deformation magnitude (shading deformation)
  float expb = exp(-betaShading);
  return (alphaShading*u)/(expb+u*(1.0-expb));
}

vec2 firstOrderDeformation(in vec2 coord,in float anchorDepth,in float currentDepth) {
  // compute deformed coordinates according to the difference of depths
  vec2 offset = fp(currentDepth-anchorDepth)*(coord-anchorPoint);
  return coord+offset;
}

vec2 secondOrderDeformation(in vec2 coord,in vec3 anchorNormal,in vec3 currentNormal) {
  // compute deformed coordinates according to the difference of projected normals
  vec2 offset = fn(length(currentNormal.xy-anchorNormal.xy))*(currentNormal.xy-anchorNormal.xy);
  return coord+offset;
}

vec2 computeDeformedCoords(in vec2 coord,in vec4 anchorShape,in vec4 currentShape) {
  // apply global+first+second order deformation 
  vec2 newCoord = coord;
  newCoord = firstOrderDeformation(newCoord,anchorShape.w,currentShape.w);
  newCoord = secondOrderDeformation(newCoord,anchorShape.xyz,currentShape.xyz);
  newCoord = globalDeformation(newCoord);
  return newCoord;
}

void main() {
  // normal+depth under the anchor point and under the current fragment
  vec4 anchorShape  = texture(object,anchorPoint);
  vec4 currentShape = texture(object,texcoord);

  // outside the object surface
  if(currentShape.xyz==vec3(0)) {
    outbuff = vec4(0);
    return;
  }

  // deform initial coordinates
  vec2 coord = computeDeformedCoords(texcoord,anchorShape,currentShape);

  // evaluate and display the deformed input image
  outbuff = evaluate(coord,currentShape);
}
