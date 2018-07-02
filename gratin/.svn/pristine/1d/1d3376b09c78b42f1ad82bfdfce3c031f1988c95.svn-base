// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef IMGCUSTOM_H
#define IMGCUSTOM_H

#include <QObject>
#include "imgCustomWidget.h"
#include "core/nodeHandle.h"
#include "misc/gpuProgram.h"

class ImgCustomNode : public GenericCustomNode {
 public:
  ImgCustomNode(PbGraph *parent=NULL,NodeHandle *handle=NULL,unsigned int nbInputs=1,unsigned int nbOutputs=1);
  ~ImgCustomNode();


  inline void mousePressEvent(const Vector2f &m,QMouseEvent *);
  inline void mouseMoveEvent (const Vector2f &m,QMouseEvent *);

  void apply ();
  void reload();

  inline NodeWidget *widget() {return _w;}

  bool save(SceneSaver  *saver);
  bool load(SceneLoader *loader);

 public:
  inline QString  head()    const {return _headText;}
  inline QString  body()    const {return _bodyText;}

  void setHead(const QString &head);
  void setBody(const QString &body);

 private:
  void initShaderSource();
  void initProg();

  ImgCustomWidget *_w;

  QString      _headText;
  QString      _bodyText;
  QString      _vertText;
  QString      _fragText;
  GPUProgram  *_p;
};

class ImgCustomHandle : public QObject, public NodeHandle {
  Q_OBJECT
  Q_INTERFACES(NodeHandle)

 public:
  ImgCustomHandle();

  const QString     name();       // node name
  const QString     path();       // node path
  const QString     desc();       // node description
  const QString     help();       // node help
  const QStringList inputDesc();  // description of each input image
  const QStringList outputDesc(); // description of each output image

  // creates a new instance of this node
  NodeInterface *createInstance(PbGraph *parent,unsigned int nbInputs,unsigned int nbOutputs);
};

inline void ImgCustomNode::mousePressEvent(const Vector2f &m,QMouseEvent *) {
  _w->setMousePosition(m);
}

inline void ImgCustomNode::mouseMoveEvent(const Vector2f &m,QMouseEvent *) {
  _w->setMousePosition(m);
}


#endif // IMGCUSTOM_H
