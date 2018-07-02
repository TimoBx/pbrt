// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef IMGCROP_H
#define IMGCROP_H

#include <QObject>
#include "core/nodeHandle.h"
#include "core/nodeTexture2D.h"
#include "misc/gpuProgram.h"

class ImgCropNode : public NodeTexture2D {
 public:
  // from nodeInterface
  ImgCropNode(PbGraph *parent=NULL,NodeHandle *handle=NULL);

  inline unsigned int nbInputs()  const {return 1;}
  inline unsigned int nbOutputs() const {return 1;}
  inline bool outputSizeEqualInputSize() const {return false;}

  void apply ();
  void reload();
  void init();

  inline NodeWidget *widget() {return _w;}
  
  bool save(SceneSaver  *saver);
  bool load(SceneLoader *loader);
 
 public:
  // from ImageCropNode
  void setImgSize(const Vector2i &size);
  inline const Vector2i currentSize() const {return Vector2i(outputSize()[0],outputSize()[1]);}

 private:
  GPUProgram  _p;
  NodeWidget *_w;
  bool _sizeInitialized;
};

class ImgCropHandle : public QObject, public NodeHandle {
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



#endif // IMGCROP_H
