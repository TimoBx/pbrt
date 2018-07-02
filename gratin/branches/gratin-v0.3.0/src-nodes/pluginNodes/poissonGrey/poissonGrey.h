// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
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
  NodeWidget(node),_nbCycles(new IntSliderWidget(node,"nb cycles",1,10,1)) {
    QVBoxLayout *l = new QVBoxLayout();
    l->addWidget(_nbCycles);
    setLayout(l);
    addChildWidget(_nbCycles);
  }
  
  inline IntSliderWidget *nbCycles() const {return _nbCycles;}

 private:
  IntSliderWidget *_nbCycles;
};

// **** the node itself, containing the shader and the widget ****
class PoissonGreyNode : public NodeTexture2D {
 public:
  PoissonGreyNode(PbGraph *parent=NULL,NodeHandle *handle=NULL);
  void apply();
  void reload();
  void init();
  void clean();

  inline NodeWidget  *widget() {return _w;} // the widget 

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
class PoissonGreyHandle : public QObject, public NodeHandleInterface {
  Q_OBJECT
  Q_INTERFACES(NodeHandleInterface)
  Q_PLUGIN_METADATA(IID "Gratin.PoissonGrey")

 public:
  const QString     stringId() const {return "poissonGreyId";}
  unsigned int      version () const {return 1;}

  const QString     name    () const {return "imgPoissonGrey";}
  const QString     path    () const {return "plugins/";}
  const QString     desc    () const {return "Poisson reconstruction with Dirichlet constraints";}
  const QString     help    () const {return tr("Poisson reconstruction<br>"
						"This node is waiting for (gx,gy,d,use_d)<br>"
						"where gx, gy is the input gradient and <br>"
						"d is a Dirichlet constraint<br>"
						"The Dirichlet constraint won't be taken into account if use_d==0");}
  
  const QStringList inputNames () const {return (QStringList() << "gradient");}
  const QStringList outputNames() const {return (QStringList() << "diffused");}
  
  
  NodeInterface *createInstance(PbGraph *parent) {
    return new PoissonGreyNode(parent,new NodeHandle(this));
  }

  NodeInterface *createInstance(PbGraph *parent,
				const QString &name,
				const QString &path,
				const QString &desc,
				const QString &help,
				const QStringList &inputNames,
				const QStringList &outputNames) {    
    return new PoissonGreyNode(parent,new NodeHandle(stringId(),version(),name,path,desc,help,
						     inputNames,outputNames));
  }
};

#endif // POISSON_GREY_H
