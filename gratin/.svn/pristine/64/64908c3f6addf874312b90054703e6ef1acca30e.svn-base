// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "imgLoader.h"
#include <QString>
#include <QStringList>
#include <iostream>

#include "misc/glutils.h"
#include "core/pbgraph.h"
#include "core/textureIO.h"
#include "imgLoaderWidget.h"

using namespace std;

ImgLoaderNode::ImgLoaderNode(PbGraph *parent,NodeHandle *handle)
: NodeTexture2D(parent,handle),
  _p(QString(plugDir()+"/textureNodes/imgLoader/imgLoader.vert").toStdString(),
     QString(plugDir()+"/textureNodes/imgLoader/imgLoader.frag").toStdString()),
  _filename(":/default-tex"),
  _tex(Glutils::qimageToTexture(_filename)),
  _w(new ImgLoaderWidget(this)) {
  _p.addUniform("img");
}

ImgLoaderNode::~ImgLoaderNode() {
  delete _tex;
}

void ImgLoaderNode::apply() {
  Glutils::setViewport(outputTex(0)->w(),outputTex(0)->h());

  _p.enable();
  _p.setUniformTexture("img",GL_TEXTURE_2D,_tex->id());
  drawOutputs(buffersOfOutputTex(0));
  _p.disable();
}

void ImgLoaderNode::loadImage(const QString &filename,const Vector2i &size) {
  TextureIO loader;
  FloatTexture2D *t = loader.load(filename);
  if(t) {
    delete _tex;
    _tex = t;
    _p.setUniformTexture("img",GL_TEXTURE_2D,_tex->id());
    QDir d(".");
    _filename = d.relativeFilePath(filename);
    if(size==Vector2i(0,0)) 
      setOutputSize(Vector2f(t->w(),t->h()));
    else
      setOutputSize(Vector2f((float)size[0],(float)size[1]));

    ((ImgLoaderWidget *)widget())->imgSizeChanged(Vector2i(t->w(),t->h()),size);
  }
}

bool ImgLoaderNode::save(SceneSaver  *saver) {
  ImgLoaderWidget *w = (ImgLoaderWidget *)widget();
  return (saver->saveString("filename",_filename) && 
	  saver->saveVector2i("size",w->scale()->val())); 
}

bool ImgLoaderNode::load(SceneLoader *loader) {
  _filename = ":/default-tex";
  Vector2i size(0,0);
  bool ret = loader->loadString("filename",_filename) && loader->loadVector2i("size",size);
  loadImage(_filename,size);
  return ret;
}

void ImgLoaderNode::setImgSize(const Vector2i &size) {
  setOutputSize(Vector2f((float)size[0],(float)size[1]));
}

void ImgLoaderNode::reload() {
  _p.reload();
}

const QString ImgLoaderHandle::name() {
  return "imgLoader";
}

const QString ImgLoaderHandle::path() {
  return "inputs/";
}

const QString ImgLoaderHandle::desc() {
  return "Image loader";
}

const QString ImgLoaderHandle::help() {
  return tr("Load an image. <br>"
	    "Use the interface to load, resize and reset image parameters.");
}

const QStringList ImgLoaderHandle::inputDesc() {
  return QStringList();
}

const QStringList ImgLoaderHandle::outputDesc() {
  return (QStringList() << "image");
}

NodeInterface *ImgLoaderHandle::createInstance(PbGraph *parent,unsigned int,unsigned int) {
  return new ImgLoaderNode(parent,this);
}
