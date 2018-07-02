// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "gbuffersCustomWidget.h"
#include "gbuffersCustom.h"
#include "view/animationWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QScrollArea>
#include <QMessageBox>

using namespace std;

QDir GbuffersCustomWidget::_currentPath = QDir("data/");

GbuffersCustomWidget::GbuffersCustomWidget(GbuffersCustomNode *node)
  : GenericCustomWidget(node),
    _gbuffCustomNode(node),
    _tabWidget(new QTabWidget()),
    _vertHead(new QLabel()),
    _vertSource(new CodeEditor()),
    _fragHead(new QLabel()),
    _fragSource(new CodeEditor()),
    _settings(new QPushButton("Settings...")),
    _apply(new QPushButton("Apply")),
    _log(new QLabel("")),
    _load(new QPushButton("Load...")),
    _default(new QPushButton("Reset cam")) {

  QWidget     *tmp;
  QVBoxLayout *l;

  _camWidget = new KFTrackballWidget(node,"Camera",node->camera());

  // default parameters 
  userLayout()->addWidget(_load);
  userLayout()->addWidget(_camWidget);
  userLayout()->addWidget(_default);

  // code widget (1st tab)
  QScrollArea *vcodeArea = new QScrollArea();
  vcodeArea->setWidgetResizable(true);
  tmp = new QWidget();
  l = new QVBoxLayout();
  l->addWidget(_vertHead);
  l->addWidget(_vertSource);
  tmp->setLayout(l);
  vcodeArea->setWidget(tmp);

  QScrollArea *fcodeArea = new QScrollArea();
  fcodeArea->setWidgetResizable(true);
  tmp = new QWidget();
  l = new QVBoxLayout();
  l->addWidget(_fragHead);
  l->addWidget(_fragSource);
  tmp->setLayout(l);
  fcodeArea->setWidget(tmp);

  // parameters widget (2nd tab)
  QScrollArea *paramArea = new QScrollArea();
  paramArea->setWidgetResizable(true);
  tmp = new QWidget();
  tmp->setLayout(_userLayout);
  paramArea->setWidget(tmp);

  // parameters widget (2nd tab)
  QScrollArea *logArea = new QScrollArea();
  logArea->setWidgetResizable(true);
  logArea->setWidget(_log);

  _tabWidget->addTab(paramArea,"Params");
  _tabWidget->addTab(vcodeArea,"Vertex");
  _tabWidget->addTab(fcodeArea,"Fragment");
  _tabWidget->addTab(logArea,"Log");

  l = new QVBoxLayout();
  l->addWidget(_settings);
  l->addWidget(_tabWidget);
  l->addWidget(_apply);
  setLayout(l);

  _vertHead->setText(node->vertHead());
  _vertSource->setLineShift(_vertHead->text().split(QRegExp("\n|\r")).size());
  _vertSource->setPlainText(node->vertBody());
  _fragHead->setText(node->fragHead());
  _fragSource->setLineShift(_fragHead->text().split(QRegExp("\n|\r")).size());
  _fragSource->setPlainText(node->fragBody());
  

  connect(_settings,SIGNAL(clicked()),this,SLOT(settingsClicked()));
  connect(_apply,SIGNAL(clicked()),this,SLOT(applyClicked()));
  connect(_load,SIGNAL(clicked()),this,SLOT(loadClicked()));
  connect(_default,SIGNAL(clicked()),this,SLOT(defaultClicked()));

  addChildWidget(_camWidget);
}

void GbuffersCustomWidget::loadClicked() {
  QString filename = QFileDialog::getOpenFileName(0,"Load object",_currentPath.absolutePath(),"Objects (*.obj)");

  if(filename.isEmpty()) {
    return;
  }

  QDir d(".");
  _currentPath = d.filePath(filename);

  ((GbuffersCustomNode *)node())->loadObject(filename);
  updateGraph();
}

void GbuffersCustomWidget::defaultClicked() {
  ((GbuffersCustomNode *)node())->initCam();
  updateGraph();
}

void GbuffersCustomWidget::settingsClicked() {

  GenericCustomWidgetSetting settings(this,false);

  int ret = settings.exec();
  if(ret==QDialog::Rejected)
    return;

  // remove everything from the animation widget 
  AnimationWidget::instance()->delParams(this);

  initFromSettings(settings);
  QString vhead = generateHead(QObject::tr("#version 330\n\n"
					   "layout(location = 0) in vec3 inVertex;\n"
					   "layout(location = 1) in vec3 inNormal;\n"
					   "layout(location = 2) in vec3 inTangent;\n"
					   "layout(location = 3) in vec2 inTexcoord;\n\n"
					   "uniform mat4 model;\n"
					   "uniform mat4 view;\n"
					   "uniform mat4 proj;\n"
					   "uniform float zmin;\n"
					   "uniform float zmax;\n"),false);

  QString fhead = generateHead(QObject::tr("#version 330\n\n"
					   "uniform mat4 model;\n"
					   "uniform mat4 view;\n"
					   "uniform mat4 proj;\n"
					   "uniform float zmin;\n"
					   "uniform float zmax;\n"),true);

  _gbuffCustomNode->setHeads(vhead,fhead);
  _vertHead->setText(vhead);
  _fragHead->setText(fhead);

  _vertSource->setLineShift(_vertHead->text().split(QRegExp("\n|\r")).size());
  _fragSource->setLineShift(_fragHead->text().split(QRegExp("\n|\r")).size());

  updateGraph();
}

void GbuffersCustomWidget::applyClicked() {
  QString vbody = _vertSource->toPlainText();
  QString fbody = _fragSource->toPlainText();
  _gbuffCustomNode->setBodies(vbody,fbody);
  updateGraph();
}


void GbuffersCustomWidget::sendUniformsToProg(GPUProgram *p) {
  TrackballCamera *camera = _gbuffCustomNode->camera();
  p->setUniformMatrix4fv("model",(GLfloat *)camera->modelMatrix().data());
  p->setUniformMatrix4fv("view",(GLfloat *)camera->viewMatrix().data());
  p->setUniformMatrix4fv("proj",(GLfloat *)camera->projMatrix().data());
  p->setUniform1f("zmin",camera->zmin());
  p->setUniform1f("zmax",camera->zmax());

  GenericCustomWidget::sendUniformsToProg(p);
}

void GbuffersCustomWidget::setLogFromProg(GPUProgram *p) {
  string vl = p->log(VERT);  
  string fl = p->log(FRAG);
  _log->setText("Vertex shader:\n"+QString(vl.c_str())+"\n\nFragment shader:\n"+QString(fl.c_str()));
}

const QStringList GbuffersCustomWidget::getUniformNames() {
  return QStringList() << "model" << "view" << "proj" << "zmin" << "zmax" << GenericCustomWidget::getUniformNames();
}

void GbuffersCustomWidget::setVertSource(const QString &head,const QString &body) {
  _vertHead->setText(head);
  _vertSource->setLineShift(_vertHead->text().split(QRegExp("\n|\r")).size());
  _vertSource->setPlainText(body);
}

void GbuffersCustomWidget::setFragSource(const QString &head,const QString &body) {
  _fragHead->setText(head);
  _fragSource->setLineShift(_fragHead->text().split(QRegExp("\n|\r")).size());
  _fragSource->setPlainText(body);
}
