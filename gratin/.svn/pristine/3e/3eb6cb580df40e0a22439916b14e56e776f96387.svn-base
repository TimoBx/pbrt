// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <iostream>

#include "imgGlobalMean.h"
#include "misc/glutils.h"

using namespace std;

ImgGlobalMeanNode::ImgGlobalMeanNode(PbGraph *parent,NodeHandle *handle)
  : NodeTexture2D(parent,handle),
    _pinit(QString(plugDir()+"/textureNodes/imgGlobalMean/imgGlobalMean.vert").toStdString(),
	   QString(plugDir()+"/textureNodes/imgGlobalMean/imgGlobalMeanInit.frag").toStdString()),
    _pcomp(QString(plugDir()+"/textureNodes/imgGlobalMean/imgGlobalMean.vert").toStdString(),
	   QString(plugDir()+"/textureNodes/imgGlobalMean/imgGlobalMean.frag").toStdString()) {

  _pinit.addUniform("img");
  _pcomp.addUniform("img");
}

void ImgGlobalMeanNode::initFBO() {
  NodeTexture2D::initFBO();

  outputTex(0)->bind();
  _glf->glGenerateMipmap(GL_TEXTURE_2D);
  _glf->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
  _glf->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  _glf->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
  _glf->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
  _glf->glBindTexture(GL_TEXTURE_2D,0);
}

void ImgGlobalMeanNode::apply() {
  int  w      = (int)(inputTex(0)->w());
  int  h      = (int)(inputTex(0)->h());
  int  nbl    = 1;

  // fbo initialization (level 0)
  _fbo.bind();
  outputTex(0)->bind();
  _glf->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_BASE_LEVEL, 0);
  _glf->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_LEVEL,0);
  _fbo.attachTexture(GL_TEXTURE_2D,outputTex(0)->id(),*buffersOfOutputTex(0),0);
  _fbo.isValid();

  // initialization (simple copy)
  _pinit.enable();
  _pinit.setUniformTexture("img",GL_TEXTURE_2D,inputTex(0)->id());
  _pinit.disable();

  Glutils::setViewport(w,h);
  _pinit.enable();
  drawOutputs(buffersOfOutputTex(0),1,false,false);
  _pinit.disable();

  // global var computation 
  while(w!=1 && h!=1) {
    outputTex(0)->bind();
    _glf->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_BASE_LEVEL,nbl-1);
    _glf->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_LEVEL, nbl-1);

    _pcomp.enable();
    _pcomp.setUniformTexture("img",GL_TEXTURE_2D,outputTex(0)->id());
    _pcomp.disable();

    outputTex(0)->bind();
    _glf->glGetTexLevelParameteriv(GL_TEXTURE_2D,nbl,GL_TEXTURE_WIDTH,&w);
    _glf->glGetTexLevelParameteriv(GL_TEXTURE_2D,nbl,GL_TEXTURE_HEIGHT,&h);
    _fbo.attachTexture(GL_TEXTURE_2D,outputTex(0)->id(),*buffersOfOutputTex(0),nbl);
    _fbo.isValid();

    Glutils::setViewport(w,h);
    _pcomp.enable();
    drawOutputs(buffersOfOutputTex(0),1,false,false);
    _pcomp.disable();

    nbl ++;
  }
  
  outputTex(0)->bind();
  _glf->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_BASE_LEVEL, 0);
  _glf->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_LEVEL,nbl);

  FramebufferObject::unbind();
}

void ImgGlobalMeanNode::reload() {
  _pinit.reload();
  _pcomp.reload();
}

const QString ImgGlobalMeanHandle::name() {
  return "imgGlobalMean";
}

const QString ImgGlobalMeanHandle::path() {
  return "statistics/global/";
}

const QString ImgGlobalMeanHandle::desc() {
  return "Compute the mean color of an input image";
}

const QString ImgGlobalMeanHandle::help() {
  return tr("Compute the mean color of an input image.<br>"
	    "The result is stored in the last image level: ceil(log2(max(width,height)))+1");
}

const QStringList ImgGlobalMeanHandle::inputDesc() {
  return (QStringList() << "image");
}

const QStringList ImgGlobalMeanHandle::outputDesc() {
  return (QStringList() << "meanVal");
}

NodeInterface *ImgGlobalMeanHandle::createInstance(PbGraph *parent,unsigned int,unsigned int) {
  return new ImgGlobalMeanNode(parent,this);
}
