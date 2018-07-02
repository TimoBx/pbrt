// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef NODE_HANDLE_H
#define NODE_HANDLE_H

#include <QtPlugin>
#include <QString>
#include <QStringList>

#include "misc/glinclude.h"
#include "core/nodeInterface.h"
#include "core/pbgraph.h"

class LIBRARY_EXPORT NodeHandle {
 public:
  virtual ~NodeHandle() {}
  virtual const QString     name()       = 0; // node name (used as ID. no spaces please)
  virtual const QString     path()       = 0; // node path (no spaces please)
  virtual const QString     desc()       = 0; // node description
  virtual const QString     help()       = 0; // node help
  virtual const QStringList inputDesc()  = 0; // description of each input data
  virtual const QStringList outputDesc() = 0; // description of each output data

  // creates a new instance of this node
  virtual NodeInterface *createInstance(PbGraph *parent,unsigned int nbInputs,unsigned int nbOutputs)=0;

 private:
  QString _name;
  QString _path;
  QString _desc;
  QString _help;

};

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(NodeHandle,"Gratin.NodeHandle/1.0")
QT_END_NAMESPACE

#endif // NODE_HANDLE_H
