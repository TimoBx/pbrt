// This file is part of Gratin, a programmable Node-based System
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef GLSLOADER_H
#define GLSLOADER_H

#include <Patate/vitelotte.h>

#include <QObject>
#include "genericCustomNode.h"
#include "genericCustomWidget.h"
#include "core/nodeHandle.h"
#include "misc/gpuProgram.h"
#include "misc/vertexarrayObject.h"

class MVGLoaderNode;

class MVGLoaderWidget : public GenericCustomWidget {
  Q_OBJECT

public:
  static bool showLoadError(std::string& msg, unsigned line,
							MVGLoaderWidget* loaderWidget);

public:
  MVGLoaderWidget(MVGLoaderNode *node);

public slots:
  void loadClicked();
  void defaultClicked();
  void setWireframe(bool on);

private:
  QPushButton           *_load;
  QCheckBox             *_wireframe;
  QPushButton           *_default;
  TrackballCameraWidget *_camWidget;
  static QDir            _currentPath;

  friend class MVGLoaderNode;
};


class MVGLoaderNode : public GenericCustomNode {
 public:
  MVGLoaderNode(PbGraph *parent=NULL,NodeHandle *handle=NULL);
  ~MVGLoaderNode();

  void apply ();

  void mousePressEvent(const Vector2f &m,QMouseEvent *);
  void mouseMoveEvent (const Vector2f &m,QMouseEvent *);
  void wheelEvent     (const Vector2f &,QWheelEvent *);

  inline NodeWidget *widget() {return _w;}

  virtual const QString constantVertHead();
  virtual const QString defaultVertBody(); 
  virtual const QString constantFragHead();
  virtual const QString defaultFragBody(); 
  virtual const QString constantTessCHead();
  virtual const QString defaultTessCBody(); 
  virtual const QString constantTessEHead();
  virtual const QString defaultTessEBody(); 
  virtual const QString constantGeomHead(); 
  virtual const QString defaultGeomBody(); 

  bool save(SceneSaver  *saver);
  bool load(SceneLoader *loader);

  void loadObject(const QString &filename, MVGLoaderWidget *widget=NULL);
  void initCam();
  void setShowWireframe(bool on);
  
  inline bool showWireframe() {return _showWireframe;}
  inline TrackballCamera *camera() {return _camera;}

 private:
  Vector3f toPosition(const Eigen::VectorXf& v) const;
  Vector4f toColor(const Eigen::VectorXf& v) const;

 protected:
  const QStringList defaultUniformNames() const { 
    return QStringList() << "viewMatrix" << "projMatrix"
			 << "zmin" << "zmax" << "viewportSize" << "nodes"
			 << "baseNodeIndex" << "singularTriangles"
			 << "enableShading"  << "smoothness"
			 << "showWireframe"; }
  const QStringList defaultUniformTypes() const { 
    return QStringList() << "mat4" << "mat4"
			 << "float" << "float" << "vec2" << "samplerBuffer"
			 << "int" << "bool"
			 << "bool" << "float"
			 << "bool"; }

  void initFBO();
  void cleanFBO();

 private:
  QString               _filename;
  TrackballCamera      *_camera;
  VertexarrayObject    *_vao;
  FloatTexture2D       *_depth;
  GLuint                _nodesBuffer;
  GLuint                _nodesTexture;
  MVGLoaderWidget      *_w;
  unsigned int          _nTriangles;
  unsigned int          _nSingulars;
  float                 _smoothness;
  bool                  _is3d;
  bool                  _isQuadratic;
  bool                  _showWireframe;
};

class MVGLoaderHandle : public QObject, public NodeHandleInterface {
  Q_OBJECT
  Q_INTERFACES(NodeHandleInterface)
  Q_PLUGIN_METADATA(IID "Gratin.MVGLoader")

 public:
  const QString     stringId() const {return "MVGLoaderId";}
  unsigned int      version () const {return 1;}

  const QString     name    () const {return "MVGLoader";}
  const QString     path    () const {return "Geometry/";}
  const QString     desc    () const {return "Load mvg files";}
  const QString     help    () const {
    return tr("This node loads mvg file (Mesh-based vector graphics). It is based on\n"
	      "the vitelotte module of Patate Lib (http://patate.gforge.inria.fr).\n\n"
	      "This node supports 2D and 3D files, computes color diffusion with FV\n"
	      "elements if required and supports MVG files with diffusion curves.\n\n"
	      "This node produce two output: a color buffer and a normal/depth buffer.");
  }
  
  const QStringList inputNames () const {return QStringList();}
  const QStringList outputNames() const {return QStringList() << "color" << "normal";}
  
  
  NodeInterface *createInstance(PbGraph *parent) {
    return new MVGLoaderNode(parent,new NodeHandle(this));
  }

  NodeInterface *createInstance(PbGraph *parent,
				const QString &name,
				const QString &path,
				const QString &desc,
				const QString &help,
				const QStringList &inputNames,
				const QStringList &outputNames) {    
    return new MVGLoaderNode(parent,new NodeHandle(stringId(),version(),name,path,desc,help,
						   inputNames,outputNames));
  }
};


#endif //GLSLOADER
