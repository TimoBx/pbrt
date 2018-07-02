// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "view/logWidget.h"

#include <QPalette>
#include <QFont>

LogWidget *LogWidget::_instance = NULL;

LogWidget::LogWidget()
  : QLabel() {

  QPalette palette;
  QBrush brush(QColor(0,0,0,255));
  brush.setStyle(Qt::SolidPattern);
  palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
  palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);

  //set palette
  setPalette(palette);

  //set font
  QFont font;
  font.setPointSize(10);
  font.setBold(false);

  setFont(font);
  setAutoFillBackground(true);
  setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  setSizePolicy(QSizePolicy(QSizePolicy::Minimum,QSizePolicy::Maximum));
}

LogWidget::~LogWidget() {

}
