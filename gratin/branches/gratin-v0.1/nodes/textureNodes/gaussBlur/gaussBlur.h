// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef GAUSSBLUR_H
#define GAUSSBLUR_H

#include <QObject>
#include <QVBoxLayout>
#include "core/nodeHandle.h"
#include "core/nodeTexture2D.h"
#include "view/widgetParams.h"


// **** a widget containing a keyframed slider ****
class GaussBlurWidget : public NodeWidget {
 public:
 GaussBlurWidget(NodeInterface *node) : 
  NodeWidget(node),_halfsize(new KFSliderInt(node,"size",0,500,0)) {
    QVBoxLayout *l = new QVBoxLayout();
    l->addWidget(_halfsize);
    setLayout(l);
    addChildWidget(_halfsize);
  }
  
  inline KFSliderInt *halfsize() const {return _halfsize;}

 private:
  KFSliderInt *_halfsize;
};

// **** the node itself, containing the shader and the widget ****
class GaussBlurNode : public NodeTexture2D {
 public:
  GaussBlurNode(PbGraph *parent=NULL,NodeHandle *handle=NULL);
  void apply();
  void reload();

  inline NodeWidget  *widget   ()        {return _w;} // the widget 
  inline unsigned int nbInputs ()  const {return 1; } // input data 
  inline unsigned int nbOutputs()  const {return 1; } // output data 

 protected:
  inline unsigned int nbTmps   ()  const {return 1; } // tmp data (gauss blur needs 2 passes)

 private:
  GPUProgram       _p;
  GaussBlurWidget *_w;
};

// **** the node handle, containing information displayed in the interface ****
class GaussBlurHandle : public QObject, public NodeHandle {
  Q_OBJECT
  Q_INTERFACES(NodeHandle)
  Q_PLUGIN_METADATA(IID "Gratin.GaussBlur")

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

#endif // HELLOEX_H
