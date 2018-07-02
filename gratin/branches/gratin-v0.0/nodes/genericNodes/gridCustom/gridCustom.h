// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef GRIDCUSTOM_H
#define GRIDCUSTOM_H

#include <QObject>
#include "gridCustomWidget.h"
#include "core/nodeHandle.h"
#include "misc/gpuProgram.h"
#include "misc/vertexbufferObject.h"

class GridCustomNode : public GenericCustomNode {
 public:
  GridCustomNode(PbGraph *parent=NULL,NodeHandle *handle=NULL,unsigned int nbInputs=0,unsigned int nbOutputs=1);
  ~GridCustomNode();

  void mousePressEvent(const Vector2f &m,QMouseEvent *);
  void mouseMoveEvent (const Vector2f &m,QMouseEvent *);
  void wheelEvent     (const Vector2f &,QWheelEvent *);

  void apply ();
  void reload();

  inline NodeWidget *widget() {return _w;}

  bool save(SceneSaver  *saver);
  bool load(SceneLoader *loader);

 public:
  inline QString  vertHead()    const {return _vertHeadText;}
  inline QString  vertBody()    const {return _vertBodyText;}
  inline QString  fragHead()    const {return _fragHeadText;}
  inline QString  fragBody()    const {return _fragBodyText;}
  inline TrackballCamera *camera() {return _camera;}

  void setHeads(const QString &vertHead,const QString &fragHead);
  void setBodies(const QString &vertBody,const QString &fragBody);
  void setImgSize(const Vector2f &tsize,const Vector2f &toffset);

  void initGrid();
  void initCam();
  
 protected:
  void initFBO();
  void cleanFBO();

 private:
  void initShaderSource();
  void initProg();

  TrackballCamera      *_camera;
  VertexbufferObject   *_vbo;
  FloatTexture2D       *_depth;
  GridCustomWidget     *_w;

  QString      _vertHeadText;
  QString      _vertBodyText;
  QString      _fragHeadText;
  QString      _fragBodyText;
  QString      _vertText;
  QString      _fragText;
  GPUProgram  *_p;
};

class GridCustomHandle : public QObject, public NodeHandle {
  Q_OBJECT
  Q_INTERFACES(NodeHandle)

 public:
  GridCustomHandle();

  const QString     name();       // node name
  const QString     path();       // node path
  const QString     desc();       // node description
  const QString     help();       // node help
  const QStringList inputDesc();  // description of each input image
  const QStringList outputDesc(); // description of each output image

  // creates a new instance of this node
  NodeInterface *createInstance(PbGraph *parent,unsigned int nbInputs,unsigned int nbOutputs);
};


#endif // GRIDCUSTOM_H
