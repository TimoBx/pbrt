// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "imgCrop.h"
#include <QString>
#include <QStringList>
#include <iostream>

#include "misc/glutils.h"
#include "core/pbgraph.h"
#include "core/textureIO.h"
#include "imgCropWidget.h"

using namespace std;

ImgCropNode::ImgCropNode(PbGraph *parent,NodeHandle *handle)
: NodeTexture2D(parent,handle),
  _p(QString(plugDir()+"/textureNodes/imgCrop/imgCrop.vert").toStdString(),
     QString(plugDir()+"/textureNodes/imgCrop/imgCrop.frag").toStdString()),
  _w(new ImgCropWidget(this)),
  _sizeInitialized(false) {
  _p.addUniform("img");
  _p.addUniform("pos");
}

void ImgCropNode::apply() {
  Glutils::setViewport(outputTex(0)->w(),outputTex(0)->h());

  _p.enable();
  _p.setUniformTexture("img",GL_TEXTURE_2D,inputTex(0)->id());
  _p.setUniform2iv("pos",((ImgCropWidget *)widget())->cropPos()->val().data());
  drawOutputs(buffersOfOutputTex(0));
  _p.disable();
}

void ImgCropNode::init() {
  ImgCropWidget *w  = (ImgCropWidget *)widget();
  FloatTexture2D *t = inputTex(0);
  if(!_sizeInitialized) {
    setOutputSize(Vector2f(t->w(),t->h()));
    w->imgSizeChanged(Vector2i(t->w(),t->h()),Vector2i(0,0));
  } else {
    w->imgSizeChanged(Vector2i(t->w(),t->h()),w->cropSize()->val());
  }
  _sizeInitialized = true;
}

bool ImgCropNode::save(SceneSaver  *saver) {
  ImgCropWidget *w = (ImgCropWidget *)widget();
  return saver->saveVector2i("cropSize",w->cropSize()->val()); 
}

bool ImgCropNode::load(SceneLoader *loader) {
  _sizeInitialized = true;
  Vector2i size(0,0);
  bool ret = loader->loadVector2i("cropSize",size);
  setImgSize(size);
  return ret;
}

void ImgCropNode::setImgSize(const Vector2i &size) {
  setOutputSize(Vector2f((float)size[0],(float)size[1]));
}

void ImgCropNode::reload() {
  _p.reload();
}

const QString ImgCropHandle::name() {
  return "imgCrop";
}

const QString ImgCropHandle::path() {
  return "transform/";
}

const QString ImgCropHandle::desc() {
  return "Crop an input image";
}

const QString ImgCropHandle::help() {
  return tr("Crop an image. <br>"
	    "Use the interface to crop and reset input image parameters.");
}

const QStringList ImgCropHandle::inputDesc() {
  return QStringList();
}

const QStringList ImgCropHandle::outputDesc() {
  return (QStringList() << "cropped");
}

NodeInterface *ImgCropHandle::createInstance(PbGraph *parent,unsigned int,unsigned int) {
  return new ImgCropNode(parent,this);
}

