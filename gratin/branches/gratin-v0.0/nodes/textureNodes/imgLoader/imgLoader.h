// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef IMGLOADER_H
#define IMGLOADER_H

#include <QObject>
#include "core/nodeHandle.h"
#include "core/nodeTexture2D.h"
#include "misc/gpuProgram.h"

class ImgLoaderNode : public NodeTexture2D {
 public:
  // from nodeInterface
  ImgLoaderNode(PbGraph *parent=NULL,NodeHandle *handle=NULL);
  ~ImgLoaderNode();

  inline unsigned int nbInputs()  const {return 0;}
  inline unsigned int nbOutputs() const {return 1;}

  void apply ();
  void reload();

  inline NodeWidget *widget() {return _w;}
  
  bool save(SceneSaver  *saver);
  bool load(SceneLoader *loader);
 
 public:
  // from ImageLoaderNode
  void loadImage(const QString &filename,const Vector2i &size=Vector2i(0,0));
  void setImgSize(const Vector2i &size);
  inline FloatTexture2D *tex() {return _tex;}

 private:
  GPUProgram      _p;
  QString         _filename;
  FloatTexture2D *_tex;
  NodeWidget     *_w;
};

class ImgLoaderHandle : public QObject, public NodeHandle {
  Q_OBJECT
  Q_INTERFACES(NodeHandle)

 public:
  const QString     name();       // node name
  const QString     path();       // node path
  const QString     desc();       // node description
  const QString     help();       // node help
  const QStringList inputDesc();  // description of each input image
  const QStringList outputDesc(); // description of each output image

  // creates a new instance of this node
  NodeInterface *createInstance(PbGraph *parent,unsigned int nbInputs,unsigned int nbOutputs);
};



#endif // IMGLOADER_H
