// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2015-2016 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <iostream>

#include "distanceMap.h"
using namespace std;

DistanceMapNode::DistanceMapNode(PbGraph *parent,NodeHandle *handle)
  : NodeTexture2D(parent,handle),
    _distProg(":/distanceMap.vert",":/distanceMap.frag"),
    _nbPass(0) {
    _distProg.addUniform("distSrc");
    _distProg.addUniform("numPass");
}

void DistanceMapNode::init() {
  _nbPass = (unsigned int)(1.+floor(log2((double)std::max(outputTex(0)->w(),outputTex(0)->h()))));
  _outbuff[  _nbPass%2] = buffersOfTmpTex(0);
  _outbuff[1-_nbPass%2] = buffersOfOutputTex(0);
  _inSampl[  _nbPass%2] = outputTex(0);
  _inSampl[1-_nbPass%2] = tmpTex(0);
}

void DistanceMapNode::reload() {
  _distProg.reload();
}

void DistanceMapNode::receiveEvent(const NodeEvent &event) {
  NodeTexture2D::receiveEvent(event);
  if(event.type()==NodeEvent::OUTPUT_SIZE_HAS_CHANGED) {
    if(isReady()) init();
  }
}

void DistanceMapNode::apply() {
  // set viewport 
  Glutils::setViewport(outputTex(0)->w(),outputTex(0)->h());

  _distProg.enable();
  
  // first pass (use input texture)
  _distProg.setUniform1i("numPass",0);
  _distProg.setUniformTexture("distSrc",GL_TEXTURE_2D,inputTex(0)->id());
  drawOutputs(_outbuff[0],1,true,false);

  // the rest of the passes
  for(unsigned int i=1;i<_nbPass;++i) {
    _distProg.setUniform1i("numPass",(int)i);
    _distProg.setUniformTexture("distSrc",GL_TEXTURE_2D,_inSampl[i%2]->id());
    drawOutputs(_outbuff[i%2],1,false,false);
  }

  // disable everything
  _distProg.disable();
  FramebufferObject::unbind();
}
