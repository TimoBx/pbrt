// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2015-2016 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef DISTANCEMAP_H
#define DISTANCEMAP_H

#include "core/nodeHandle.h"
#include "core/nodeTexture2D.h"

// **** the node itself, containing shaders and widgets ****
class DistanceMapNode : public NodeTexture2D {
 public:
  DistanceMapNode(PbGraph *parent=NULL,NodeHandle *handle=NULL);
  ~DistanceMapNode() {};

  void init();
  void apply();
  void receiveEvent(const NodeEvent &event);
  void reload();

 protected:
  // need 1 tmp texture
  inline unsigned int nbTmps()  const {return 1;} 

 private:
  GPUProgram      _distProg;
  GLenum         *_outbuff[2];
  FloatTexture2D *_inSampl[2];
  unsigned int    _nbPass;
};




// **** the node handle, containing information displayed in the interface ****
class DistanceMapHandle : public QObject, public NodeHandleInterface {
  Q_OBJECT
    Q_INTERFACES(NodeHandleInterface)
    Q_PLUGIN_METADATA(IID "Gratin.DistanceMap")

    public:
  const QString     stringId() const {return "distanceMapId";}
  unsigned int      version () const {return 1;}

  const QString     name    () const {return "distanceMap";}
  const QString     path    () const {return "Analysis/";}
  const QString     desc    () const {return "Euclidian distance to closest black input pixels";}
  const QString     help    () const {return tr("Compute the distance to the closest black input pixels (when the first channel is 0)\n"
						"The output map contains for each pixel, the coordinate to the closest seed (two first channels)\n"
						"and the corresponding distance (third channel)");}
  
  const QStringList inputNames () const {return QStringList() << "image"; }
  const QStringList outputNames() const {return (QStringList() << "distToSeed");}
  
  NodeInterface *createInstance(PbGraph *parent) {
    return new DistanceMapNode(parent,new NodeHandle(this));
  }

  NodeInterface *createInstance(PbGraph *parent,
				const QString &name,
				const QString &path,
				const QString &desc,
				const QString &help,
				const QStringList &inputNames,
				const QStringList &outputNames) {    
    return new DistanceMapNode(parent,new NodeHandle(stringId(),version(),name,path,desc,help,
						   inputNames,outputNames));
  }
};

#endif // DISTANCEMAP_H
