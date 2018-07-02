// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef SFIMAGETOOL_H
#define SFIMAGETOOL_H

#include <QObject>
#include <QVBoxLayout>
#include "core/nodeHandle.h"
#include "core/nodeTexture2D.h"
#include "view/widgetParams.h"


// **** a widget ****
class SfImageToolWidget : public NodeWidget {
 public:
  SfImageToolWidget(NodeInterface *node);
  
  inline KFSliderFloat *alphaShading() const {return _alphaShading;}
  inline KFSliderFloat *betaShading () const {return _betaShading;}
  inline KFSliderFloat *gammaTexture() const {return _gammaTexture;}
  inline KFSliderFloat *gammaCorrect() const {return _gammaCorrect;}
  inline KFColorPicker *colorPicker () const {return _colorPicker;}
  inline KFEdit2Float  *translation () const {return _translation;}
  inline KFSliderFloat *rotation    () const {return _rotation;}
  inline KFSliderFloat *scaling     () const {return _scaling;}
  inline ComboBoxInt   *type        () const {return _type;}
  inline ComboBoxInt   *mode        () const {return _mode;}

 private:
  KFSliderFloat *_alphaShading;
  KFSliderFloat *_betaShading;
  KFSliderFloat *_gammaTexture;
  KFSliderFloat *_gammaCorrect;
  ComboBoxInt   *_type;
  ComboBoxInt   *_mode;
  KFColorPicker *_colorPicker;
  QGroupBox     *_deformBox;
  QGroupBox     *_behaviorBox;
  QGroupBox     *_materialBox;
  KFEdit2Float  *_translation;
  KFSliderFloat *_rotation;
  KFSliderFloat *_scaling;
};

// **** the node itself, containing the shader and the widget ****
class SfImageToolNode : public NodeTexture2D {
 public:
  SfImageToolNode(PbGraph *parent=NULL,NodeHandle *handle=NULL);
  void apply();
  void reload();

  inline NodeWidget  *widget   ()        {return _w;}
  inline unsigned int nbInputs ()  const {return 2; }
  inline unsigned int nbOutputs()  const {return 1; }
  
  void mousePressEvent(const Vector2f &m,QMouseEvent *e);
  void mouseMoveEvent (const Vector2f &m,QMouseEvent *e);

 private:
  GPUProgram         _p;
  SfImageToolWidget *_w;


  enum {TRANSLATE,ROTATE,SCALE};
  Vector2f           _tmp;     // just used for mouse interaction
  int                _interact;// just used for mouse interaction
};



// **** the node handle, containing information displayed in the interface ****
class SfImageToolHandle : public QObject, public NodeHandle {
  Q_OBJECT
  Q_INTERFACES(NodeHandle)
  Q_PLUGIN_METADATA(IID "Gratin.SfImageTool")

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
