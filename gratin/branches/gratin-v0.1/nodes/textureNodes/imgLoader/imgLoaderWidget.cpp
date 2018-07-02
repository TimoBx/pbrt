// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "imgLoaderWidget.h"
#include "imgLoader.h"
#include "core/textureIO.h"
#include <QVBoxLayout>

#define MAX_TEX_SIZE 8192

ImgLoaderWidget::ImgLoaderWidget(NodeInterface *node)
  : NodeWidget(node) {
  
  FloatTexture2D *img = ((ImgLoaderNode *)node)->tex();
  _origImageSize = img ? Vector2i(img->w(),img->h()) : Vector2i(512,512);
  _currentScale  = _origImageSize;
  Vector2i maxSize(max(_origImageSize[0],MAX_TEX_SIZE),max(_origImageSize[1],MAX_TEX_SIZE));
  Vector2i minSize(1,1);

  _load    = new QPushButton("Load...");
  _default = new QPushButton("Reset");
  _scaleBox = new QGroupBox("Resize");
  _aspectRatio = new QCheckBox("Keep original aspect ratio");
  _scaleSize = new Edit2Int(node,"Size",minSize,maxSize,_origImageSize);

  QVBoxLayout *l;

  l = new QVBoxLayout();
  l->addWidget(_aspectRatio);
  l->addWidget(_scaleSize);
  _scaleBox->setLayout(l);

  l = new QVBoxLayout();
  l->addWidget(_load);
  l->addWidget(_scaleBox);
  l->addWidget(_default);
  setLayout(l);

  connect(_load,SIGNAL(clicked()),this,SLOT(loadClicked()));
  connect(_default,SIGNAL(clicked()),this,SLOT(defaultClicked()));
  connect(_scaleSize,SIGNAL(valChanged()),this,SLOT(scaleSizeChanged()));

  addChildWidget(_scaleSize);
}

void ImgLoaderWidget::loadClicked() {
  TextureIO loader;
  QString fileName = loader.askForLoadParams();
  if(!fileName.isEmpty()) {
    ((ImgLoaderNode *)node())->loadImage(fileName);
    updateGraph();
  }
}

void ImgLoaderWidget::defaultClicked() {
  _scaleSize->setVal(_origImageSize,false);
  updateGraph();
}

void ImgLoaderWidget::scaleSizeChanged() {
  Vector2i newSize = _scaleSize->val();

  if(_aspectRatio->checkState()==Qt::Checked) {
    const double ratio = (double)_origImageSize[0]/(double)_origImageSize[1];
    if(_currentScale[0]==newSize[0]) {
      newSize[0] = max((int)(ratio*(double)newSize[1]),1);
    } else {
      newSize[1] = max((int) ((double)newSize[0]/ratio),1 );
    }
    _scaleSize->setVal(newSize,false);
  } 

  _currentScale = newSize;
  ((ImgLoaderNode *)node())->setImgSize(_currentScale);
}

void ImgLoaderWidget::imgSizeChanged(const Vector2i &orig,const Vector2i &scale) {
  _origImageSize = orig;
  const Vector2i tmp  = scale==Vector2i(0,0) ? orig : scale;
  _scaleSize->setVal(tmp,false);
}
