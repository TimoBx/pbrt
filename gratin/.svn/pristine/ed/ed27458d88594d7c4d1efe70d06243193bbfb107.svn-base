// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef IMGGLOBALMIN_H
#define IMGGLOBALMIN_H

#include <QObject>
#include <QVBoxLayout>
#include "core/nodeHandle.h"
#include "core/nodeTexture2D.h"

// **** the node itself, containing the shader ****
class ImgGlobalMinNode : public NodeTexture2D {
 public:
  ImgGlobalMinNode(PbGraph *parent=NULL,NodeHandle *handle=NULL);
  void apply();
  void reload();

  inline unsigned int nbInputs ()  const {return 1;} 
  inline unsigned int nbOutputs()  const {return 1;} 

 protected:
  void initFBO();

 private:
  GPUProgram _pinit;
  GPUProgram _pcomp;
};

// **** the node handle, containing information displayed in the interface ****
class ImgGlobalMinHandle : public QObject, public NodeHandle {
  Q_OBJECT
  Q_INTERFACES(NodeHandle)

 public:
  const QString     name();       // node name
  const QString     path();       // node path
  const QString     desc();       // node description
  const QString     help();       // node help
  const QStringList inputDesc();  // description of each input image
  const QStringList outputDesc(); // description of each output image

  // create a new instance of this node
  NodeInterface *createInstance(PbGraph *parent,unsigned int nbInputs,unsigned int nbOutputs);
};

#endif // IMGGLOBALMIN_H
