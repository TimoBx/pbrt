// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2015-2016 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "multiImgLoaderWidget.h"
#include "multiImgLoader.h"
#include "core/textureIO.h"
#include <QVBoxLayout>

#define MAX_TEX_SIZE 8192

MultiImgLoaderWidget::MultiImgLoaderWidget(NodeInterface *node)
  : NodeWidget(node),
    _update(true) {
  
  _origImageSize = ((MultiImgLoaderNode *)node)->currentOrigSize();
  _currentScale  = ((MultiImgLoaderNode *)node)->currentSize();
  
  Vector2i maxSize(max(_origImageSize[0],MAX_TEX_SIZE),max(_origImageSize[1],MAX_TEX_SIZE));
  Vector2i minSize(1,1);

  _load        = new QPushButton("Load...");
  _default     = new QPushButton("Reset");
  _scaleBox    = new QGroupBox("Resize");
  _aspectRatio = new QCheckBox("Keep original aspect ratio");
  _loadAll     = new QCheckBox("Load all images in memory");
  _resizeAll   = new QCheckBox("Resize all images when size is changing");
  _scaleSize   = new Vector2iSpinWidget(node,"Size",minSize,maxSize,_origImageSize,false);
  _currentImage= new IntSliderWidget(node,"current",0,
				     ((MultiImgLoaderNode *)node)->nbImages()-1,
				     ((MultiImgLoaderNode *)node)->currentIndex());
  QVBoxLayout *l;

  l = new QVBoxLayout();
  l->addWidget(_aspectRatio);
  l->addWidget(_loadAll);
  l->addWidget(_resizeAll);
  l->addWidget(_scaleSize);
  _scaleBox->setLayout(l);

  l = new QVBoxLayout();
  l->addWidget(_load);
  l->addWidget(_scaleBox);
  l->addWidget(_default);
  l->addWidget(_currentImage);
  l->addStretch(1);
  setLayout(l);

  connect(_load,SIGNAL(clicked()),this,SLOT(loadClicked()));
  connect(_default,SIGNAL(clicked()),this,SLOT(defaultClicked()));
  connect(_scaleSize,SIGNAL(valChanged()),this,SLOT(scaleSizeChanged()));
  connect(_loadAll,SIGNAL(stateChanged(int)),this,SLOT(loadAllChanged()));

  addChildWidget(_scaleSize);
  addChildWidget(_currentImage);
}

void MultiImgLoaderWidget::loadClicked() {
  TextureIO loader;
  const QStringList fileNames = loader.askForMultipleLoadParams();

  std::vector<QString> f;
  for(int i=0;i<fileNames.size();++i) {
    f.push_back(fileNames[i]);
  }

  if(!f.empty()) {
    ((MultiImgLoaderNode *)node())->loadImages(f);
    updateGraph();
  }
}

void MultiImgLoaderWidget::defaultClicked() {
  _scaleSize->setVal(_origImageSize,false);
  updateGraph();
}

void MultiImgLoaderWidget::scaleSizeChanged() {
  if(!_update) return;

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
  ((MultiImgLoaderNode *)node())->setImgSize(_currentScale,resizeAllImages());
}

void MultiImgLoaderWidget::currentImageChanged() {
  if(!_update) return;

  _update = false;
  unsigned int c = (unsigned int)(_currentImage->val());
  ((MultiImgLoaderNode *)node())->setCurrentIndex(c);
  
  _origImageSize = ((MultiImgLoaderNode *)node())->currentOrigSize();
  _currentScale  = ((MultiImgLoaderNode *)node())->currentSize();
  _scaleSize->setVal(_currentScale,false);

  _update = true;
}


void MultiImgLoaderWidget::imgSizeChanged(const Vector2i &orig,const Vector2i &scale) {
  _origImageSize = orig;
  const Vector2i tmp  = scale==Vector2i(0,0) ? orig : scale;
  _scaleSize->setVal(tmp,false);
}

void MultiImgLoaderWidget::setSliderVals(int nb,int current) {
  _update = false;
  
  _currentImage->setMax(nb-1);
  _currentImage->setVal(current);

  _update = true;
}

void MultiImgLoaderWidget::loadAllChanged() {
  ((MultiImgLoaderNode *)node())->loadAllChanged();
}

bool MultiImgLoaderWidget::save(SceneSaver  *saver ) {
  bool a = _aspectRatio->checkState()==Qt::Checked;
  bool r = _resizeAll->checkState()==Qt::Checked;
  bool l = _loadAll->checkState()==Qt::Checked;

  bool ret = true;

  if(!saver->saveBool("keepAspectRatio",a)) ret = false;
  if(!saver->saveBool("resizeAllImages",r)) ret = false;
  if(!saver->saveBool("loadAllImages",l))   ret = false;
  if(!NodeWidget::save(saver))              ret = false;

  return ret;
}

bool MultiImgLoaderWidget::load(SceneLoader *loader) {
  bool a = false;
  bool r = false;
  bool l = false;
  bool ret = true;

  if(!loader->loadBool("keepAspectRatio",a)) ret = false;
  if(!loader->loadBool("resizeAllImages",r)) ret = false;
  if(!loader->loadBool("loadAllImages",l))   ret = false;
  if(!NodeWidget::load(loader))              ret = false;
  
  _aspectRatio->setCheckState(a ? Qt::Checked : Qt::Unchecked);
  _resizeAll->setCheckState(r ? Qt::Checked : Qt::Unchecked);
  _loadAll->setCheckState(l ? Qt::Checked : Qt::Unchecked);

  return ret;
}
