// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <iostream>

#include "gaussBlur.h"
#include "misc/glutils.h"

using namespace std;

GaussBlurNode::GaussBlurNode(PbGraph *parent,NodeHandle *handle)
  : NodeTexture2D(parent,handle),
    _p(QString(plugDir()+"/textureNodes/gaussBlur/gaussBlur.vert").toStdString(),
       QString(plugDir()+"/textureNodes/gaussBlur/gaussBlur.frag").toStdString()),
    _w(new GaussBlurWidget(this)) {

    _p.addUniform("img");
    _p.addUniform("direction");
    _p.addUniform("halfsize");
}

void GaussBlurNode::apply() {

  // set viewport 
  Glutils::setViewport(outputTex(0)->w(),outputTex(0)->h());

  // first pass 
  _p.enable();
  _p.setUniformTexture("img",GL_TEXTURE_2D,inputTex(0)->id());
  _p.setUniform1i("halfsize",_w->halfsize()->val());
  _p.setUniform1i("direction",0);
  drawOutputs(buffersOfTmpTex(0),1,true,false);

  // second pass 
  _p.setUniformTexture("img",GL_TEXTURE_2D,tmpTex(0)->id());
  _p.setUniform1i("direction",1);
  drawOutputs(buffersOfOutputTex(0),1,false,true);
  _p.disable();
}

void GaussBlurNode::reload() {
  _p.reload();
}

const QString GaussBlurHandle::name() {
  return "imgGaussianBlur";
}

const QString GaussBlurHandle::path() {
  return "filters/blur/";
}

const QString GaussBlurHandle::desc() {
  return "Apply a gaussien blur on an input image";
}

const QString GaussBlurHandle::help() {
  return tr("The amount of blur is controled by the kernel size.<br>"
	    "Increase this size to increase the blur");
}

const QStringList GaussBlurHandle::inputDesc() {
  return (QStringList() << "image");
}

const QStringList GaussBlurHandle::outputDesc() {
  return (QStringList() << "blurred");
}

NodeInterface *GaussBlurHandle::createInstance(PbGraph *parent,unsigned int,unsigned int) {
  return new GaussBlurNode(parent,this);
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(helloEx,GaussBlurHandle)
QT_END_NAMESPACE

