// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef IMGCUSTOM_WIDGET
#define IMGCUSTOM_WIDGET

#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QTabWidget>

#include "genericCustomWidget.h"
#include "codeEditor.h"

class ImgCustomNode;

class ImgCustomWidget : public GenericCustomWidget {
  Q_OBJECT
    
    public:
  ImgCustomWidget(ImgCustomNode *node);

  public slots:
  void settingsClicked();
  void applyClicked();

 public:
  void setLogFromProg(GPUProgram *p);
  void setSource(const QString &head,const QString &body);

 private:
  ImgCustomNode *_imgCustomNode;
  QTabWidget    *_tabWidget;
  QLabel        *_head;
  //CodeEditor    *_head;
  CodeEditor    *_source;
  QPushButton   *_settings;
  QPushButton   *_apply;
  QLabel        *_log;
};

#endif // IMGCUSTOM_WIDGET
