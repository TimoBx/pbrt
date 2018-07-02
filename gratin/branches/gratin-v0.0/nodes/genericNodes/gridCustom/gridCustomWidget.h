// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef GRIDCUSTOM_WIDGET
#define GRIDCUSTOM_WIDGET

#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QTabWidget>
#include <QDir>

#include "genericCustomWidget.h"
#include "codeEditor.h"

class GridCustomNode;

class GridCustomWidget : public GenericCustomWidget {
  Q_OBJECT

    public:
  GridCustomWidget(GridCustomNode *node);

  inline SliderInt *gridDepth() {return _gridDepth;}

  public slots:
  void settingsClicked();
  void applyClicked();
  void gridDepthChanged();
  void defaultClicked();

 public:
  void setLogFromProg(GPUProgram *p);
  void setVertSource(const QString &head,const QString &body);
  void setFragSource(const QString &head,const QString &body);
  void sendUniformsToProg(GPUProgram *p);

 protected:
  const QStringList getUniformNames();

 private:
  GridCustomNode *_gbuffCustomNode;
  QTabWidget    *_tabWidget;
  QLabel        *_vertHead;
  CodeEditor    *_vertSource;
  QLabel        *_fragHead;
  CodeEditor    *_fragSource;
  QPushButton   *_settings;
  QPushButton   *_apply;
  QLabel        *_log;

  KFSliderInt       *_gridDepth;
  QPushButton       *_default;
  KFTrackballWidget *_camWidget;
  static QDir        _currentPath;
};


#endif // GRIDCUSTOM_WIDGET
