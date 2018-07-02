// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef POISSONGREY_H
#define POISSONGREY_H

#include <QObject>
#include <QVBoxLayout>
#include <QCheckBox>
#include "core/nodeHandle.h"
#include "core/nodeTexture2D.h"
#include "view/widgetParams.h"
#include "poissonSolverBW.h"


// **** a widget containing a keyframed slider ****
class PoissonGreyWidget : public NodeWidget {
 public:
 PoissonGreyWidget(NodeInterface *node) : 
  NodeWidget(node),_nbCycles(new SliderInt(node,"nb cycles",1,10,1)) {
    QVBoxLayout *l = new QVBoxLayout();
    l->addWidget(_nbCycles);
    setLayout(l);
    addChildWidget(_nbCycles);
  }
  
  inline SliderInt *nbCycles() const {return _nbCycles;}

 private:
  SliderInt *_nbCycles;
};

// **** the node itself, containing the shader and the widget ****
class PoissonGreyNode : public NodeTexture2D {
 public:
  PoissonGreyNode(PbGraph *parent=NULL,NodeHandle *handle=NULL);
  void apply();
  void reload();
  void init();
  void clean();

  inline NodeWidget  *widget   ()        {return _w;} // the widget 
  inline unsigned int nbInputs ()  const {return 1; } // input data 
  inline unsigned int nbOutputs()  const {return 1; } // output data 

  // BEGIN TMP
  void keyPressEvent(QKeyEvent *ke);
  // END TMP 

 private:
  PoissonGreyWidget *_w;
  PoissonSolverBW    _p;
  bool _initialized;

  // BEGIN TMP 
  enum {RESIDUAL=0,TMP=1,SOLUTION=2};
  int        _debugMode;
  int        _debugLevel;
  GPUProgram _debugProgram;
  void test();
  // END TMP 
};

// **** the node handle, containing information displayed in the interface ****
class PoissonGreyHandle : public QObject, public NodeHandle {
  Q_OBJECT
  Q_INTERFACES(NodeHandle)
  Q_PLUGIN_METADATA(IID "Gratin.ImgPoissonGrey")

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
