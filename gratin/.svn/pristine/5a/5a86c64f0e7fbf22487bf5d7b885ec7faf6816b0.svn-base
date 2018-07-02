// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef IMGLOADER_WIDGET
#define IMGLOADER_WIDGET

#include <QWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QCheckBox>
#include "view/nodeWidget.h"
#include "view/widgetParams.h"

class ImgLoaderWidget : public NodeWidget {
  Q_OBJECT

    public:
  ImgLoaderWidget(NodeInterface *node);

  void imgSizeChanged(const Vector2i &orig,const Vector2i &scale=Vector2i(0,0));

  inline Edit2Int *scale() {return _scaleSize;}

  public slots:
  void loadClicked();
  void defaultClicked();
  void scaleSizeChanged();

 private:
  QPushButton   *_load;
  QPushButton   *_default;
  QGroupBox     *_scaleBox;
  QCheckBox     *_aspectRatio;
  Edit2Int      *_scaleSize;
  Vector2i       _origImageSize;
  Vector2i       _currentScale;
};

#endif // IMGLOADER_WIDGET
