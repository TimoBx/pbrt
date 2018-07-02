// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "imgCustomWidget.h"
#include "imgCustom.h"
#include "view/animationWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QScrollArea>
#include <QMessageBox>

using namespace std;

ImgCustomWidget::ImgCustomWidget(ImgCustomNode *node)
  : GenericCustomWidget(node),
    _imgCustomNode(node),
    _tabWidget(new QTabWidget()),
    _head(new QLabel()),
    //_head(new CodeEditor(0,true)),
    _source(new CodeEditor()),
    _settings(new QPushButton("Settings...")),
    _apply(new QPushButton("Apply")),
    _log(new QLabel("")) {

  QWidget     *tmp;
  QVBoxLayout *l;

  // code widget (1st tab)
  QScrollArea *codeArea = new QScrollArea();
  codeArea->setWidgetResizable(true);
  tmp = new QWidget();
  l = new QVBoxLayout();
  l->addWidget(_head);
  l->addWidget(_source);
  tmp->setLayout(l);
  codeArea->setWidget(tmp);

  // parameters widget (2nd tab)
  QScrollArea *paramArea = new QScrollArea();
  paramArea->setWidgetResizable(true);
  tmp = new QWidget();
  tmp->setLayout(userLayout());
  paramArea->setWidget(tmp);

  // parameters widget (2nd tab)
  QScrollArea *logArea = new QScrollArea();
  logArea->setWidgetResizable(true);
  logArea->setWidget(_log);

  _tabWidget->addTab(paramArea,"Params");
  _tabWidget->addTab(codeArea,"Source");
  _tabWidget->addTab(logArea,"Log");

  l = new QVBoxLayout();
  l->addWidget(_settings);
  l->addWidget(_tabWidget);
  l->addWidget(_apply);
  setLayout(l);

  _head->setText(node->head());
  _source->setLineShift(_head->text().split(QRegExp("\n|\r")).size());
  _source->setPlainText(node->body());

  //_source->setLineShift(_head->document()->blockCount());

  connect(_settings,SIGNAL(clicked()),this,SLOT(settingsClicked()));
  connect(_apply,SIGNAL(clicked()),this,SLOT(applyClicked()));
}

void ImgCustomWidget::settingsClicked() {

  GenericCustomWidgetSetting settings(this,true);

  int ret = settings.exec();
  if(ret==QDialog::Rejected)
    return;

  // remove everything from the animation widget 
  AnimationWidget::instance()->delParams(this);

  initFromSettings(settings);
  QString head = generateHead(QObject::tr("#version 330\n\n"
					  "in vec2 texcoord;\n\n"),true);

  _imgCustomNode->setHead(head);
  _head->setText(head);
  _source->setLineShift(_head->text().split(QRegExp("\n|\r")).size());
  //_source->setLineShift(_head->document()->blockCount());
  updateGraph();
}

void ImgCustomWidget::applyClicked() {
  QString body = _source->toPlainText();
  _imgCustomNode->setBody(body);
  updateGraph();
}

void ImgCustomWidget::setLogFromProg(GPUProgram *p) {
  string l = p->log(FRAG);
  _log->setText(QString(l.c_str()));
}

void ImgCustomWidget::setSource(const QString &head,const QString &body) {
  _head->setText(head);
  //_source->setLineShift(_head->document()->blockCount());
  _source->setLineShift(_head->text().split(QRegExp("\n|\r")).size());
  _source->setPlainText(body);
}
