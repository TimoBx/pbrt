// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2015-2016 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef MULTIIMGLOADER_H
#define MULTIIMGLOADER_H

#include <QObject>
#include "core/nodeHandle.h"
#include "core/nodeTexture2D.h"
#include "misc/gpuProgram.h"

class MultiImgLoaderWidget;

class MultiImgLoaderNode : public NodeTexture2D {
 public:
  // from nodeInterface
  MultiImgLoaderNode(PbGraph *parent=NULL,NodeHandle *handle=NULL);
  ~MultiImgLoaderNode();

  void apply ();
  void reload();

  inline NodeWidget *widget() {return (NodeWidget *)_w;}
  inline FloatTexture2D *currentTex() {return _texs[_current];}

  inline Vector2i currentSize() {return _sizes[_current];}
  inline Vector2i currentOrigSize() {return Vector2i(currentTex()->w(),currentTex()->h());}

  inline unsigned int nbImages() const {return _texs.size();}
  inline unsigned int currentIndex() const {return _current;}

  void setCurrentIndex(unsigned int index);
  
  bool save(SceneSaver  *saver);
  bool load(SceneLoader *loader);
 
 public:
  // from ImageLoaderNode
  void loadImages(const std::vector<QString> &filenames,const std::vector<Vector2i> &sizes=std::vector<Vector2i>(),int current=-1);
  void setImgSize(const Vector2i &size,bool applyToAll=false);
  void loadAllChanged();


 private:
  GPUProgram                    _p;
  std::vector<QString>          _filenames;
  std::vector<FloatTexture2D *> _texs;
  std::vector<Vector2i>         _sizes;
  MultiImgLoaderWidget         *_w;
  unsigned int                  _current;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

class MultiImgLoaderHandle : public QObject, public NodeHandleInterface {
  Q_OBJECT
  Q_INTERFACES(NodeHandleInterface)
  Q_PLUGIN_METADATA(IID "Gratin.MultiImgLoader")

 public:
  const QString     stringId() const {return "multiImgLoaderId";}
  unsigned int      version () const {return 1;}

  const QString     name    () const {return "multiImageLoader";}
  const QString     path    () const {return "Imaging/";}
  const QString     desc    () const {return "Multiple Image loader";}
  const QString     help    () const {return tr("Load one or more images.\n",
						"Use the interface to load, resize and reset image parameters.\n"
						"The slider allows to navigate through loaded images.\n");}
  
  const QStringList inputNames () const {return QStringList();}
  const QStringList outputNames() const {return (QStringList() << "image");}
  
  
  NodeInterface *createInstance(PbGraph *parent) {
    return new MultiImgLoaderNode(parent,new NodeHandle(this));
  }

  NodeInterface *createInstance(PbGraph *parent,
				const QString &name,
				const QString &path,
				const QString &desc,
				const QString &help,
				const QStringList &inputNames,
				const QStringList &outputNames) {    
    return new MultiImgLoaderNode(parent,new NodeHandle(stringId(),version(),name,path,desc,help,
						   inputNames,outputNames));
  }
};



#endif // MULTIIMGLOADER_H
