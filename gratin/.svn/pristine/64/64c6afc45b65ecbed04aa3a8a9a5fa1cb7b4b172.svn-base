// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2015-2016 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef MULTIIMGLOADER_WIDGET
#define MULTIIMGLOADER_WIDGET

#include <QWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QCheckBox>
#include "view/nodeWidget.h"
#include "view/widgetParams.h"

class MultiImgLoaderWidget : public NodeWidget {
  Q_OBJECT

    public:
  MultiImgLoaderWidget(NodeInterface *node);

  void imgSizeChanged(const Vector2i &orig,const Vector2i &scale=Vector2i(0,0));
  void setSliderVals(int nb,int current);

  inline Vector2iSpinWidget *scale() {return _scaleSize;}
  inline bool loadAllImages() const {return _loadAll->checkState()==Qt::Checked;}
  inline bool resizeAllImages() const {return _resizeAll->checkState()==Qt::Checked;}

  // load and save 
  bool save(SceneSaver  *saver );
  bool load(SceneLoader *loader);

  void currentImageChanged();
  inline bool testImageChanged(int current) const {return _currentImage->val()!=current;}
  

  public slots:
  void loadClicked();
  void defaultClicked();
  void scaleSizeChanged();
  void loadAllChanged();
  
 private:
  QPushButton   *_load;
  QPushButton   *_default;
  QGroupBox     *_scaleBox;
  QCheckBox     *_aspectRatio;
  QCheckBox     *_resizeAll;
  QCheckBox     *_loadAll;
  Vector2iSpinWidget *_scaleSize;
  IntSliderWidget *_currentImage;
  Vector2i       _origImageSize;
  Vector2i       _currentScale;
  bool           _update;
};

#endif // MULTIIMGLOADER_WIDGET
