// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "view/widgetParams.h"

#include <iostream>
#include <QHBoxLayout>
#include <QVBoxLayout>

using namespace std;

// *****************************************************
// ***************** SliderFloat class *****************
// *****************************************************
SliderFloat::SliderFloat(NodeInterface *node,const QString &name,float minVal,float maxVal,float value,int precision)
  : NodeWidget(node),
    _maxVal(maxVal),
    _minVal(minVal),
    _scale(1000.0f),
    _update(true),
    _precision(precision) {
 
  _currentVal = max(min(value,maxVal),minVal);
  _slider = new QSlider(Qt::Horizontal);
  _slider->setMinimum(0);
  _slider->setMaximum((int)_scale);
  _slider->setValue(floatToInt(_currentVal));

  _nameLabel = new QLabel(name);
  _valLabel  = new QLabel(QString::number(_currentVal,'f',_precision));

  QHBoxLayout *l = new QHBoxLayout();
  l->addWidget(_nameLabel);
  l->addWidget(_slider);
  l->addWidget(_valLabel);
  setLayout(l);
  
  connect(_slider,SIGNAL(valueChanged(int)),this,SLOT(valueChanged(int)));
}

void SliderFloat::valueChanged(int val) {
  _currentVal = intToFloat(val);
  emit valChanged();
  _valLabel->setText(QString::number(_currentVal,'f',_precision));
  if(_update) {
    updateGraph();
  }
}

bool SliderFloat::save(SceneSaver *saver) {
  return (saver->saveFloat("sliderVal",_currentVal) && 
	  saver->saveFloat("sliderMinVal",_minVal) && 
	  saver->saveFloat("sliderMaxVal",_maxVal) && 
	  NodeWidget::save(saver));
}

bool SliderFloat::load(SceneLoader *loader) {
  bool ret = (loader->loadFloat("sliderVal",_currentVal) && 
	      loader->loadFloat("sliderMinVal",_minVal) && 
	      loader->loadFloat("sliderMaxVal",_maxVal) && 
	      NodeWidget::load(loader));
  setVal(_currentVal,false);
  return ret;
}


// *****************************************************
// **************** KFSliderFloat class ****************
// *****************************************************
KFSliderFloat::KFSliderFloat(NodeInterface *node,const QString &name,float minVal,float maxVal,float value,int precision)
  : SliderFloat(node,name,minVal,maxVal,value,precision),
    _kf(minVal,maxVal,value) {
  
  _editButton = new QPushButton("edit");
  _setButton  = new QPushButton("set");
  layout()->addWidget(_editButton);
  layout()->addWidget(_setButton);

  connect(_editButton,SIGNAL(clicked()),this,SLOT(editClicked()));
  connect(_setButton,SIGNAL(clicked()),this,SLOT(setClicked()));
  connect(_slider,SIGNAL(valueChanged(int)),this,SLOT(checkChangeCurve()));
}

void KFSliderFloat::editClicked() {
  AnimationWidget::instance()->addParam(this,_kf.curve(),QStringList() << name(),_minVal,_maxVal);
  AnimationWidget::instance()->show();
}

void KFSliderFloat::setClicked() {
  float current = (float)AnimationWidget::instance()->currentFrame();
  _kf.set(_currentVal);
  _kf.curve()->addKeyframe(Vector2f(current,_kf.get()));
  AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name());
}

void KFSliderFloat::checkChangeCurve() {
  if(_kf.curve()->nbPoints()<2) {
    const Vector2f p(_kf.curve()->point()[0],_currentVal);
    _kf.curve()->setKeyframe(p);
    AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name());
  }
}


bool KFSliderFloat::save(SceneSaver *saver) {
  return saver->saveIOData("keyframedFloat",&_kf) && SliderFloat::save(saver);
}

bool KFSliderFloat::load(SceneLoader *loader) {
  return loader->loadIOData("keyframedFloat",&_kf) && SliderFloat::load(loader);
}


// *****************************************************
// ****************** SliderInt class ******************
// *****************************************************
SliderInt::SliderInt(NodeInterface *node,const QString &name,int minVal,int maxVal,int value)
  : NodeWidget(node),
    _maxVal(maxVal),
    _minVal(minVal),
    _update(true) {
 
  _currentVal = max(min(value,maxVal),minVal);
  _slider = new QSlider(Qt::Horizontal);
  _slider->setMinimum(minVal);
  _slider->setMaximum(maxVal);
  _slider->setValue(_currentVal);

  _nameLabel = new QLabel(name);
  _valLabel  = new QLabel(QString::number(_currentVal));

  QHBoxLayout *l = new QHBoxLayout();
  l->addWidget(_nameLabel);
  l->addWidget(_slider);
  l->addWidget(_valLabel);
  setLayout(l);
  
  connect(_slider,SIGNAL(valueChanged(int)),this,SLOT(valueChanged(int)));
}

void SliderInt::valueChanged(int val) {
  _currentVal = val;
  emit valChanged();
  _valLabel->setText(QString::number(_currentVal));
  if(_update) {
    updateGraph();
  }
}

bool SliderInt::save(SceneSaver *saver) {
  return (saver->saveInt("sliderVal",_currentVal) && 
	  saver->saveInt("sliderMinVal",_minVal) &&
	  saver->saveInt("sliderMaxVal",_maxVal) &&
	  NodeWidget::save(saver));
}

bool SliderInt::load(SceneLoader *loader) {
  bool ret = (loader->loadInt("sliderVal",_currentVal) && 
	      loader->loadInt("sliderMinVal",_minVal) && 
	      loader->loadInt("sliderMaxVal",_maxVal) && 
	      NodeWidget::load(loader));
  
  int v = _currentVal;
  _slider->setMinimum(_minVal);
  _slider->setMaximum(_maxVal);
  setVal(v,false);

  // TODO: WARNING: the setMinimum/Maximum certainly calls the modification slot and 
  // then also modifies the _currentVal -> need a tmp variable for each widgets 

  return ret;
}


// *****************************************************
// **************** KFSliderInt class ****************
// *****************************************************
KFSliderInt::KFSliderInt(NodeInterface *node,const QString &name,int minVal,int maxVal,int value)
  : SliderInt(node,name,minVal,maxVal,value),
  _kf((float)minVal,(float)maxVal,(float)value) {
  
  _editButton = new QPushButton("edit");
  _setButton  = new QPushButton("set");
  layout()->addWidget(_editButton);
  layout()->addWidget(_setButton);

  connect(_editButton,SIGNAL(clicked()),this,SLOT(editClicked()));
  connect(_setButton,SIGNAL(clicked()),this,SLOT(setClicked()));
  connect(_slider,SIGNAL(valueChanged(int)),this,SLOT(checkChangeCurve()));
}

void KFSliderInt::editClicked() {
  AnimationWidget::instance()->addParam(this,_kf.curve(),QStringList() << name(),_minVal,_maxVal);
  AnimationWidget::instance()->show();
}

void KFSliderInt::setClicked() {
  float current = (float)AnimationWidget::instance()->currentFrame();
  _kf.set(_currentVal);
  _kf.curve()->addKeyframe(Vector2f(current,_kf.get()));
  AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name());
}

void KFSliderInt::checkChangeCurve() {
  if(_kf.curve()->nbPoints()<2) {
    const Vector2f p(_kf.curve()->point()[0],_currentVal);
    _kf.curve()->setKeyframe(p);
    AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name());
  }
}

bool KFSliderInt::save(SceneSaver *saver) {
  return saver->saveIOData("keyframedFloat",&_kf) && SliderInt::save(saver);
}

bool KFSliderInt::load(SceneLoader *loader) {
  return loader->loadIOData("keyframedFloat",&_kf) && SliderInt::load(loader);
}



// *****************************************************
// ****************** LabelFloat class *****************
// *****************************************************
LabelFloat::LabelFloat(NodeInterface *node,const QString &name,float value,int precision)
  : NodeWidget(node),
    _precision(precision) {

  _currentVal = value;
  _nameLabel  = new QLabel(name);
  _valLabel   = new QLabel(QString::number(_currentVal,'f',_precision));

  QHBoxLayout *l = new QHBoxLayout();
  l->addWidget(_nameLabel);
  l->addWidget(_valLabel);
  setLayout(l);
}

bool LabelFloat::save(SceneSaver *saver) {
  return saver->saveFloat("labelfloat",_currentVal) && NodeWidget::save(saver);
}

bool LabelFloat::load(SceneLoader *loader) {
  bool ret = loader->loadFloat("labelfloat",_currentVal) && NodeWidget::load(loader);
  setVal(_currentVal,false);
  return ret;
}


// *****************************************************
// ***************** KFLabelFloat class ****************
// *****************************************************
KFLabelFloat::KFLabelFloat(NodeInterface *node,const QString &name,float value,float maxAbsVal,int precision)
  : LabelFloat(node,name,value,precision),
    _kf(-maxAbsVal,maxAbsVal,value),
    _maxAbsVal(maxAbsVal) {

  _editButton = new QPushButton("edit");
  _setButton  = new QPushButton("set");
  layout()->addWidget(_editButton);
  layout()->addWidget(_setButton);

  connect(_editButton,SIGNAL(clicked()),this,SLOT(editClicked()));
  connect(_setButton,SIGNAL(clicked()),this,SLOT(setClicked()));
}


void KFLabelFloat::editClicked() {
  AnimationWidget::instance()->addParam(this,_kf.curve(),QStringList() << name(),-_maxAbsVal,_maxAbsVal);
  AnimationWidget::instance()->show();
}

void KFLabelFloat::setClicked() {
  float current = (float)AnimationWidget::instance()->currentFrame();
  _kf.set(_currentVal);
  _kf.curve()->addKeyframe(Vector2f(current,_kf.get()));
  AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name());
}

bool KFLabelFloat::save(SceneSaver *saver) {
  return (saver->saveIOData("keyframedFloat",&_kf) && LabelFloat::save(saver));
}

bool KFLabelFloat::load(SceneLoader *loader) {
  return (loader->loadIOData("keyframedFloat",&_kf) && LabelFloat::load(loader));
}




// *****************************************************
// ***************** Label2Float class *****************
// *****************************************************
Label2Float::Label2Float(NodeInterface *node,const QString &name,const Vector2f &value,int precision)
  : NodeWidget(node),
    _precision(precision) {

  _currentVal = value;
  _nameLabel  = new QLabel(name);
  _val1Label  = new QLabel(QString::number(_currentVal[0],'f',_precision));
  _val2Label  = new QLabel(QString::number(_currentVal[1],'f',_precision));

  QHBoxLayout *l = new QHBoxLayout();
  l->addWidget(_nameLabel);
  l->addWidget(_val1Label);
  l->addWidget(_val2Label);
  setLayout(l);
}

bool Label2Float::save(SceneSaver *saver) {
  return saver->saveVector2f("label2float",_currentVal) && NodeWidget::save(saver);
}

bool Label2Float::load(SceneLoader *loader) {
  bool ret = loader->loadVector2f("label2float",_currentVal) && NodeWidget::load(loader);
  setVal(_currentVal,false);
  return ret;
}


// *****************************************************
// **************** KFLabel2Float class ****************
// *****************************************************
KFLabel2Float::KFLabel2Float(NodeInterface *node,const QString &name,const Vector2f &value,float maxAbsVal,int precision)
  : Label2Float(node,name,value,precision),
    _kfX(-maxAbsVal,maxAbsVal,value[0]),
    _kfY(-maxAbsVal,maxAbsVal,value[1]),
    _maxAbsVal(maxAbsVal) {

  _editButton = new QPushButton("edit");
  _setButton  = new QPushButton("set");
  layout()->addWidget(_editButton);
  layout()->addWidget(_setButton);

  connect(_editButton,SIGNAL(clicked()),this,SLOT(editClicked()));
  connect(_setButton,SIGNAL(clicked()),this,SLOT(setClicked()));
}


void KFLabel2Float::editClicked() {
  AnimationWidget::instance()->addParam(this,_kfX.curve(),QStringList() << name() << "X",-_maxAbsVal,_maxAbsVal);
  AnimationWidget::instance()->addParam(this,_kfY.curve(),QStringList() << name() << "Y",-_maxAbsVal,_maxAbsVal);
  AnimationWidget::instance()->show();
}

void KFLabel2Float::setClicked() {
  float current = (float)AnimationWidget::instance()->currentFrame();
  _kfX.set(_currentVal[0]);
  _kfY.set(_currentVal[1]);
  _kfX.curve()->addKeyframe(Vector2f(current,_kfX.get()));
  _kfY.curve()->addKeyframe(Vector2f(current,_kfY.get()));
  AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "X");
  AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "Y");
}

bool KFLabel2Float::save(SceneSaver *saver) {
  return (saver->saveIOData("keyframedFloatX",&_kfX) &&
          saver->saveIOData("keyframedFloatY",&_kfY) &&
          Label2Float::save(saver));
}

bool KFLabel2Float::load(SceneLoader *loader) {
  return (loader->loadIOData("keyframedFloatX",&_kfX) &&
          loader->loadIOData("keyframedFloatY",&_kfY) &&
          Label2Float::load(loader));
}


// *****************************************************
// ***************** Label3Float class *****************
// *****************************************************
Label3Float::Label3Float(NodeInterface *node,const QString &name,const Vector3f &value,int precision)
  : NodeWidget(node),
    _precision(precision) {

  _currentVal = value;
  _nameLabel  = new QLabel(name);
  _val1Label  = new QLabel(QString::number(_currentVal[0],'f',_precision));
  _val2Label  = new QLabel(QString::number(_currentVal[1],'f',_precision));
  _val3Label  = new QLabel(QString::number(_currentVal[2],'f',_precision));

  QHBoxLayout *l = new QHBoxLayout();
  l->addWidget(_nameLabel);
  l->addWidget(_val1Label);
  l->addWidget(_val2Label);
  l->addWidget(_val3Label);
  setLayout(l);
}

bool Label3Float::save(SceneSaver *saver) {
  return saver->saveVector3f("label3float",_currentVal) && NodeWidget::save(saver);
}

bool Label3Float::load(SceneLoader *loader) {
  bool ret = loader->loadVector3f("label3float",_currentVal) && NodeWidget::load(loader);
  setVal(_currentVal,false);
  return ret;
}


// *****************************************************
// **************** KFLabel3Float class ****************
// *****************************************************
KFLabel3Float::KFLabel3Float(NodeInterface *node,const QString &name,const Vector3f &value,float maxAbsVal,int precision)
  : Label3Float(node,name,value,precision),
    _kfX(-maxAbsVal,maxAbsVal,value[0]),
    _kfY(-maxAbsVal,maxAbsVal,value[1]),
    _kfZ(-maxAbsVal,maxAbsVal,value[2]),
    _maxAbsVal(maxAbsVal) {

  _editButton = new QPushButton("edit");
  _setButton  = new QPushButton("set");
  layout()->addWidget(_editButton);
  layout()->addWidget(_setButton);

  connect(_editButton,SIGNAL(clicked()),this,SLOT(editClicked()));
  connect(_setButton,SIGNAL(clicked()),this,SLOT(setClicked()));
}


void KFLabel3Float::editClicked() {
  AnimationWidget::instance()->addParam(this,_kfX.curve(),QStringList() << name() << "X",-_maxAbsVal,_maxAbsVal);
  AnimationWidget::instance()->addParam(this,_kfY.curve(),QStringList() << name() << "Y",-_maxAbsVal,_maxAbsVal);
  AnimationWidget::instance()->addParam(this,_kfZ.curve(),QStringList() << name() << "Z",-_maxAbsVal,_maxAbsVal);
  AnimationWidget::instance()->show();
}

void KFLabel3Float::setClicked() {
  float current = (float)AnimationWidget::instance()->currentFrame();
  _kfX.set(_currentVal[0]);
  _kfY.set(_currentVal[1]);
  _kfZ.set(_currentVal[2]);
  _kfX.curve()->addKeyframe(Vector2f(current,_kfX.get()));
  _kfY.curve()->addKeyframe(Vector2f(current,_kfY.get()));
  _kfZ.curve()->addKeyframe(Vector2f(current,_kfZ.get()));
  AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "X");
  AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "Y");
  AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "Z");
}

bool KFLabel3Float::save(SceneSaver *saver) {
  return (saver->saveIOData("keyframedFloatX",&_kfX) &&
          saver->saveIOData("keyframedFloatY",&_kfY) &&
	  saver->saveIOData("keyframedFloatZ",&_kfZ) &&
          Label3Float::save(saver));
}

bool KFLabel3Float::load(SceneLoader *loader) {
  return (loader->loadIOData("keyframedFloatX",&_kfX) &&
          loader->loadIOData("keyframedFloatY",&_kfY) &&
          loader->loadIOData("keyframedFloatZ",&_kfZ) &&
          Label3Float::load(loader));
}




// *****************************************************
// ****************** EditFloat class ******************
// *****************************************************
EditFloat::EditFloat(NodeInterface *node,const QString &name,float minVal,float maxVal,float value)
  : NodeWidget(node),
    _maxVal(maxVal),
    _minVal(minVal),
    _update(true) {
 
  _currentVal = max(min(value,maxVal),minVal);
  _edit = new QDoubleSpinBox();
  _edit->setMinimum(minVal);
  _edit->setMaximum(maxVal);
  _edit->setSingleStep((maxVal-minVal)/100.0);
  _edit->setValue(_currentVal);

  _nameLabel = new QLabel(name);

  QHBoxLayout *l = new QHBoxLayout();
  l->addWidget(_nameLabel);
  l->addWidget(_edit);
  setLayout(l);
  
  connect(_edit,SIGNAL(valueChanged(double)),this,SLOT(valueChanged(double)));
}

void EditFloat::valueChanged(double val) {
  _currentVal = val;
  emit valChanged();
  if(_update) {
    updateGraph();
  }
}

bool EditFloat::save(SceneSaver *saver) {
  return (saver->saveFloat("editVal",_currentVal) && 
	  saver->saveFloat("editMinVal",_minVal) && 
	  saver->saveFloat("editMaxVal",_maxVal) && 
	  NodeWidget::save(saver));
}

bool EditFloat::load(SceneLoader *loader) {
  bool ret = (loader->loadFloat("editVal",_currentVal) && 
	      loader->loadFloat("editMinVal",_minVal) && 
	      loader->loadFloat("editMaxVal",_maxVal) && 
	      NodeWidget::load(loader));

  _edit->setMinimum(_minVal);
  _edit->setMaximum(_maxVal);
  _edit->setSingleStep((_maxVal-_minVal)/100.0);
  setVal(_currentVal,false);
  return ret;
}



// *****************************************************
// ***************** KFEditFloat class *****************
// *****************************************************
KFEditFloat::KFEditFloat(NodeInterface *node,const QString &name,float minVal,float maxVal,float value)
  : EditFloat(node,name,minVal,maxVal,value),
    _kf(minVal,maxVal,value) {
  
  _editButton = new QPushButton("edit");
  _setButton  = new QPushButton("set");
  layout()->addWidget(_editButton);
  layout()->addWidget(_setButton);

  connect(_editButton,SIGNAL(clicked()),this,SLOT(editClicked()));
  connect(_setButton,SIGNAL(clicked()),this,SLOT(setClicked()));
  connect(_edit,SIGNAL(valueChanged(double)),this,SLOT(checkChangeCurve()));
}

void KFEditFloat::editClicked() {
  AnimationWidget::instance()->addParam(this,_kf.curve(),QStringList() << name(),_minVal,_maxVal);
  AnimationWidget::instance()->show();
}

void KFEditFloat::setClicked() {
  float current = (float)AnimationWidget::instance()->currentFrame();
  _kf.set(_currentVal);
  _kf.curve()->addKeyframe(Vector2f(current,_kf.get()));
  AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name());
}

void KFEditFloat::checkChangeCurve() {
  if(_kf.curve()->nbPoints()<2) {
    const Vector2f p(_kf.curve()->point()[0],_currentVal);
    _kf.curve()->setKeyframe(p);
    AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name());
  }
}

bool KFEditFloat::save(SceneSaver *saver) {
  return saver->saveIOData("keyframedFloat",&_kf) && EditFloat::save(saver);
}

bool KFEditFloat::load(SceneLoader *loader) {
  return loader->loadIOData("keyframedFloat",&_kf) && EditFloat::load(loader);
}



// *****************************************************
// ****************** Edit2Float class *****************
// *****************************************************
Edit2Float::Edit2Float(NodeInterface *node,const QString &name,const Vector2f &minVal,const Vector2f &maxVal,const Vector2f &value)
  : NodeWidget(node),
    _maxVal(maxVal),
    _minVal(minVal),
    _update(true) {
 
  _currentVal = Vector2f(max(min(value[0],maxVal[0]),minVal[0]),
			 max(min(value[1],maxVal[1]),minVal[1]));

  _edit1 = new QDoubleSpinBox();
  _edit1->setMinimum(minVal[0]);
  _edit1->setMaximum(maxVal[0]);
  _edit1->setSingleStep((maxVal[0]-minVal[0])/100.0);
  _edit1->setValue(_currentVal[0]);

  _edit2 = new QDoubleSpinBox();
  _edit2->setMinimum(minVal[1]);
  _edit2->setMaximum(maxVal[1]);
  _edit2->setSingleStep((maxVal[1]-minVal[1])/100.0);
  _edit2->setValue(_currentVal[1]);

  _nameLabel = new QLabel(name);

  QHBoxLayout *l = new QHBoxLayout();
  l->addWidget(_nameLabel);
  l->addWidget(_edit1);
  l->addWidget(_edit2);
  setLayout(l);
  
  connect(_edit1,SIGNAL(valueChanged(double)),this,SLOT(value1Changed(double)));
  connect(_edit2,SIGNAL(valueChanged(double)),this,SLOT(value2Changed(double)));
}

void Edit2Float::value1Changed(double val) {
  _currentVal[0] = val;
  emit valChanged();
  if(_update) {
    updateGraph();
  }
}

void Edit2Float::value2Changed(double val) {
  _currentVal[1] = val;
  emit valChanged();
  if(_update) {
    updateGraph();
  }
}

bool Edit2Float::save(SceneSaver *saver) {
  return (saver->saveVector2f("editVal",_currentVal) && 
	  saver->saveVector2f("editMinVal",_minVal) && 
	  saver->saveVector2f("editMaxVal",_maxVal) && 
	  NodeWidget::save(saver));
}

bool Edit2Float::load(SceneLoader *loader) {
  bool ret = (loader->loadVector2f("editVal",_currentVal) && 
	      loader->loadVector2f("editMinVal",_minVal) && 
	      loader->loadVector2f("editMaxVal",_maxVal) && 
	      NodeWidget::load(loader));

  _edit1->setMinimum(_minVal[0]);
  _edit1->setMaximum(_maxVal[0]);
  _edit1->setSingleStep((_maxVal[0]-_minVal[0])/100.0);
  _edit2->setMinimum(_minVal[1]);
  _edit2->setMaximum(_maxVal[1]);
  _edit2->setSingleStep((_maxVal[1]-_minVal[1])/100.0);
  setVal(_currentVal,false);
  return ret;
}



// *****************************************************
// ***************** KFEdit2Float class *****************
// *****************************************************
KFEdit2Float::KFEdit2Float(NodeInterface *node,const QString &name,const Vector2f &minVal,const Vector2f &maxVal,const Vector2f &value)
  : Edit2Float(node,name,minVal,maxVal,value),
    _kfX(minVal[0],maxVal[0],value[0]),
    _kfY(minVal[1],maxVal[1],value[1]) {
  
  _editButton = new QPushButton("edit");
  _setButton  = new QPushButton("set");
  layout()->addWidget(_editButton);
  layout()->addWidget(_setButton);

  connect(_editButton,SIGNAL(clicked()),this,SLOT(editClicked()));
  connect(_setButton,SIGNAL(clicked()),this,SLOT(setClicked()));
  connect(_edit1,SIGNAL(valueChanged(double)),this,SLOT(checkChangeCurve()));
  connect(_edit2,SIGNAL(valueChanged(double)),this,SLOT(checkChangeCurve()));
}

void KFEdit2Float::editClicked() {
  AnimationWidget::instance()->addParam(this,_kfX.curve(),QStringList() << name() << "X",_minVal[0],_maxVal[0]);
  AnimationWidget::instance()->addParam(this,_kfY.curve(),QStringList() << name() << "Y",_minVal[1],_maxVal[1]);
  AnimationWidget::instance()->show();
}

void KFEdit2Float::setClicked() {
  float current = (float)AnimationWidget::instance()->currentFrame();
  _kfX.set(_currentVal[0]);
  _kfY.set(_currentVal[1]);
  _kfX.curve()->addKeyframe(Vector2f(current,_kfX.get()));
  _kfY.curve()->addKeyframe(Vector2f(current,_kfY.get()));
  AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "X");
  AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "Y");
}

void KFEdit2Float::checkChangeCurve() {
  if(_kfX.curve()->nbPoints()<2 && _kfY.curve()->nbPoints()<2 ) {
    const Vector2f p1(_kfX.curve()->point()[0],_currentVal[0]);
    const Vector2f p2(_kfY.curve()->point()[0],_currentVal[1]);
    _kfX.curve()->setKeyframe(p1);
    _kfY.curve()->setKeyframe(p2);
    AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "X");
    AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "Y");
  }
}


bool KFEdit2Float::save(SceneSaver *saver) {
  return (saver->saveIOData("keyframedFloatX",&_kfX) && 
	  saver->saveIOData("keyframedFloatY",&_kfY) && 
	  Edit2Float::save(saver));
}

bool KFEdit2Float::load(SceneLoader *loader) {
  return (loader->loadIOData("keyframedFloatX",&_kfX) && 
	  loader->loadIOData("keyframedFloatY",&_kfY) && 
	  Edit2Float::load(loader));
}



// *****************************************************
// ****************** Edit3Float class *****************
// *****************************************************
Edit3Float::Edit3Float(NodeInterface *node,const QString &name,const Vector3f &minVal,const Vector3f &maxVal,const Vector3f &value)
  : NodeWidget(node),
    _maxVal(maxVal),
    _minVal(minVal),
    _update(true) {
 
  _currentVal = Vector3f(max(min(value[0],maxVal[0]),minVal[0]),
			 max(min(value[1],maxVal[1]),minVal[1]),
			 max(min(value[2],maxVal[2]),minVal[2]));

  _edit1 = new QDoubleSpinBox();
  _edit1->setMinimum(minVal[0]);
  _edit1->setMaximum(maxVal[0]);
  _edit1->setSingleStep((maxVal[0]-minVal[0])/100.0);
  _edit1->setValue(_currentVal[0]);

  _edit2 = new QDoubleSpinBox();
  _edit2->setMinimum(minVal[1]);
  _edit2->setMaximum(maxVal[1]);
  _edit2->setSingleStep((maxVal[1]-minVal[1])/100.0);
  _edit2->setValue(_currentVal[1]);
 
  _edit3 = new QDoubleSpinBox();
  _edit3->setMinimum(minVal[2]);
  _edit3->setMaximum(maxVal[2]);
  _edit3->setSingleStep((maxVal[2]-minVal[2])/100.0);
  _edit3->setValue(_currentVal[2]);

  _nameLabel = new QLabel(name);

  QHBoxLayout *l = new QHBoxLayout();
  l->addWidget(_nameLabel);
  l->addWidget(_edit1);
  l->addWidget(_edit2);
  l->addWidget(_edit3);
  setLayout(l);
  
  connect(_edit1,SIGNAL(valueChanged(double)),this,SLOT(value1Changed(double)));
  connect(_edit2,SIGNAL(valueChanged(double)),this,SLOT(value2Changed(double)));
  connect(_edit3,SIGNAL(valueChanged(double)),this,SLOT(value3Changed(double)));
}

void Edit3Float::value1Changed(double val) {
  _currentVal[0] = val;
  emit valChanged();
  if(_update) {
    updateGraph();
  }
}

void Edit3Float::value2Changed(double val) {
  _currentVal[1] = val;
  emit valChanged();
  if(_update) {
    updateGraph();
  }
}

void Edit3Float::value3Changed(double val) {
  _currentVal[2] = val;
  emit valChanged();
  if(_update) {
    updateGraph();
  }
}

bool Edit3Float::save(SceneSaver *saver) {
  return (saver->saveVector3f("editVal",_currentVal) && 
	  saver->saveVector3f("editMinVal",_minVal) && 
	  saver->saveVector3f("editMaxVal",_maxVal) && 
	  NodeWidget::save(saver));
}

bool Edit3Float::load(SceneLoader *loader) {
  bool ret = (loader->loadVector3f("editVal",_currentVal) && 
	      loader->loadVector3f("editMinVal",_minVal) && 
	      loader->loadVector3f("editMaxVal",_maxVal) && 
	      NodeWidget::load(loader));

  _edit1->setMinimum(_minVal[0]);
  _edit1->setMaximum(_maxVal[0]);
  _edit1->setSingleStep((_maxVal[0]-_minVal[0])/100.0);
  _edit2->setMinimum(_minVal[1]);
  _edit2->setMaximum(_maxVal[1]);
  _edit2->setSingleStep((_maxVal[1]-_minVal[1])/100.0);
  _edit3->setMinimum(_minVal[2]);
  _edit3->setMaximum(_maxVal[2]);
  _edit3->setSingleStep((_maxVal[2]-_minVal[2])/100.0);
  setVal(_currentVal,false);
  return ret;
}



// *****************************************************
// ***************** KFEdit3Float class *****************
// *****************************************************
KFEdit3Float::KFEdit3Float(NodeInterface *node,const QString &name,const Vector3f &minVal,const Vector3f &maxVal,const Vector3f &value)
  : Edit3Float(node,name,minVal,maxVal,value),
    _kfX(minVal[0],maxVal[0],value[0]),
    _kfY(minVal[1],maxVal[1],value[1]),
    _kfZ(minVal[2],maxVal[2],value[2]) {
  
  _editButton = new QPushButton("edit");
  _setButton  = new QPushButton("set");
  layout()->addWidget(_editButton);
  layout()->addWidget(_setButton);

  connect(_editButton,SIGNAL(clicked()),this,SLOT(editClicked()));
  connect(_setButton,SIGNAL(clicked()),this,SLOT(setClicked()));
  connect(_edit1,SIGNAL(valueChanged(double)),this,SLOT(checkChangeCurve()));
  connect(_edit2,SIGNAL(valueChanged(double)),this,SLOT(checkChangeCurve()));
  connect(_edit3,SIGNAL(valueChanged(double)),this,SLOT(checkChangeCurve()));
}

void KFEdit3Float::editClicked() {
  AnimationWidget::instance()->addParam(this,_kfX.curve(),QStringList() << name() << "X",_minVal[0],_maxVal[0]);
  AnimationWidget::instance()->addParam(this,_kfY.curve(),QStringList() << name() << "Y",_minVal[1],_maxVal[1]);
  AnimationWidget::instance()->addParam(this,_kfZ.curve(),QStringList() << name() << "Z",_minVal[2],_maxVal[2]);
  AnimationWidget::instance()->show();
}

void KFEdit3Float::setClicked() {
  float current = (float)AnimationWidget::instance()->currentFrame();
  _kfX.set(_currentVal[0]);
  _kfY.set(_currentVal[1]);
  _kfZ.set(_currentVal[2]);
  _kfX.curve()->addKeyframe(Vector2f(current,_kfX.get()));
  _kfY.curve()->addKeyframe(Vector2f(current,_kfY.get()));
  _kfZ.curve()->addKeyframe(Vector2f(current,_kfZ.get()));
  AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "X");
  AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "Y");
  AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "Z");
}

void KFEdit3Float::checkChangeCurve() {
  if(_kfX.curve()->nbPoints()<2 && _kfY.curve()->nbPoints()<2 && _kfZ.curve()->nbPoints()<2) {
    const Vector2f p1(_kfX.curve()->point()[0],_currentVal[0]);
    const Vector2f p2(_kfY.curve()->point()[0],_currentVal[1]);
    const Vector2f p3(_kfZ.curve()->point()[0],_currentVal[2]);
    _kfX.curve()->setKeyframe(p1);
    _kfY.curve()->setKeyframe(p2);
    _kfZ.curve()->setKeyframe(p3);
    AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "X");
    AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "Y");
    AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "Z");
  }
}

bool KFEdit3Float::save(SceneSaver *saver) {
  return (saver->saveIOData("keyframedFloatX",&_kfX) && 
	  saver->saveIOData("keyframedFloatY",&_kfY) && 
	  saver->saveIOData("keyframedFloatZ",&_kfZ) && 
	  Edit3Float::save(saver));
}

bool KFEdit3Float::load(SceneLoader *loader) {
  return (loader->loadIOData("keyframedFloatX",&_kfX) && 
	  loader->loadIOData("keyframedFloatY",&_kfY) && 
	  loader->loadIOData("keyframedFloatZ",&_kfZ) && 
	  Edit3Float::load(loader));
}


// *****************************************************
// ****************** EditInt class ******************
// *****************************************************
EditInt::EditInt(NodeInterface *node,const QString &name,int minVal,int maxVal,int value)
  : NodeWidget(node),
    _maxVal(maxVal),
    _minVal(minVal),
    _update(true) {
 
  _currentVal = max(min(value,maxVal),minVal);
  _edit = new QSpinBox();
  _edit->setMinimum(minVal);
  _edit->setMaximum(maxVal);
  _edit->setValue(_currentVal);

  _nameLabel = new QLabel(name);

  QHBoxLayout *l = new QHBoxLayout();
  l->addWidget(_nameLabel);
  l->addWidget(_edit);
  setLayout(l);
  
  connect(_edit,SIGNAL(valueChanged(int)),this,SLOT(valueChanged(int)));
}

void EditInt::valueChanged(int val) {
  _currentVal = val;
  emit valChanged();
  if(_update) {
    updateGraph();
  }
}

bool EditInt::save(SceneSaver *saver) {
  return (saver->saveInt("editVal",_currentVal) && 
	  saver->saveInt("editMinVal",_minVal) && 
	  saver->saveInt("editMaxVal",_maxVal) && 
	  NodeWidget::save(saver));
}

bool EditInt::load(SceneLoader *loader) {
  bool ret = (loader->loadInt("editVal",_currentVal) && 
	      loader->loadInt("editMinVal",_minVal) && 
	      loader->loadInt("editMaxVal",_maxVal) && 
	      NodeWidget::load(loader));
  _edit->setMinimum(_minVal);
  _edit->setMaximum(_maxVal);
  setVal(_currentVal,false);
  return ret;
}



// *****************************************************
// ***************** KFEditInt class *****************
// *****************************************************
KFEditInt::KFEditInt(NodeInterface *node,const QString &name,int minVal,int maxVal,int value)
  : EditInt(node,name,minVal,maxVal,value),
    _kf(minVal,maxVal,value) {
  
  _editButton = new QPushButton("edit");
  _setButton  = new QPushButton("set");
  layout()->addWidget(_editButton);
  layout()->addWidget(_setButton);

  connect(_editButton,SIGNAL(clicked()),this,SLOT(editClicked()));
  connect(_setButton,SIGNAL(clicked()),this,SLOT(setClicked()));
  connect(_edit,SIGNAL(valueChanged(int)),this,SLOT(checkChangeCurve()));
}

void KFEditInt::editClicked() {
  AnimationWidget::instance()->addParam(this,_kf.curve(),QStringList() << name(),_minVal,_maxVal);
  AnimationWidget::instance()->show();
}

void KFEditInt::setClicked() {
  float current = (float)AnimationWidget::instance()->currentFrame();
  _kf.set(_currentVal);
  _kf.curve()->addKeyframe(Vector2f(current,_kf.get()));
  AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name());
}

void KFEditInt::checkChangeCurve() {
  if(_kf.curve()->nbPoints()<2) {
    const Vector2f p(_kf.curve()->point()[0],_currentVal);
    _kf.curve()->setKeyframe(p);
    AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name());
  }
}

bool KFEditInt::save(SceneSaver *saver) {
  return saver->saveIOData("keyframedFloat",&_kf) && EditInt::save(saver);
}

bool KFEditInt::load(SceneLoader *loader) {
  return loader->loadIOData("keyframedFloat",&_kf) && EditInt::load(loader);
}



// *****************************************************
// ****************** Edit2Int class *****************
// *****************************************************
Edit2Int::Edit2Int(NodeInterface *node,const QString &name,const Vector2i &minVal,const Vector2i &maxVal,const Vector2i &value)
  : NodeWidget(node),
    _maxVal(maxVal),
    _minVal(minVal),
    _update(true) {
 
  _currentVal = Vector2i(max(min(value[0],maxVal[0]),minVal[0]),
			 max(min(value[1],maxVal[1]),minVal[1]));

  _edit1 = new QSpinBox();
  _edit1->setMinimum(minVal[0]);
  _edit1->setMaximum(maxVal[0]);
  _edit1->setValue(_currentVal[0]);

  _edit2 = new QSpinBox();
  _edit2->setMinimum(minVal[1]);
  _edit2->setMaximum(maxVal[1]);
  _edit2->setValue(_currentVal[1]);

  _nameLabel = new QLabel(name);

  QHBoxLayout *l = new QHBoxLayout();
  l->addWidget(_nameLabel);
  l->addWidget(_edit1);
  l->addWidget(_edit2);
  setLayout(l);
  
  connect(_edit1,SIGNAL(valueChanged(int)),this,SLOT(value1Changed(int)));
  connect(_edit2,SIGNAL(valueChanged(int)),this,SLOT(value2Changed(int)));
}

void Edit2Int::value1Changed(int val) {
  _currentVal[0] = val;
  emit valChanged();
  if(_update) {
    updateGraph();
  }
}

void Edit2Int::value2Changed(int val) {
  _currentVal[1] = val;
  emit valChanged();
  if(_update) {
    updateGraph();
  }
}

bool Edit2Int::save(SceneSaver *saver) {
  return (saver->saveVector2i("editVal",_currentVal) && 
	  saver->saveVector2i("editMinVal",_minVal) && 
	  saver->saveVector2i("editMaxVal",_maxVal) && 
	  NodeWidget::save(saver));
}

bool Edit2Int::load(SceneLoader *loader) {
  bool ret = (loader->loadVector2i("editVal",_currentVal) && 
	      loader->loadVector2i("editMinVal",_minVal) && 
	      loader->loadVector2i("editMaxVal",_maxVal) && 
	      NodeWidget::load(loader));
  _edit1->setMinimum(_minVal[0]);
  _edit1->setMaximum(_maxVal[0]);
  _edit2->setMinimum(_minVal[1]);
  _edit2->setMaximum(_maxVal[1]);
  setVal(_currentVal,false);
  return ret;
}



// *****************************************************
// ***************** KFEdit2Int class *****************
// *****************************************************
KFEdit2Int::KFEdit2Int(NodeInterface *node,const QString &name,const Vector2i &minVal,const Vector2i &maxVal,const Vector2i &value)
  : Edit2Int(node,name,minVal,maxVal,value),
    _kfX(minVal[0],maxVal[0],value[0]),
    _kfY(minVal[1],maxVal[1],value[1]) {
  
  _editButton = new QPushButton("edit");
  _setButton  = new QPushButton("set");
  layout()->addWidget(_editButton);
  layout()->addWidget(_setButton);

  connect(_editButton,SIGNAL(clicked()),this,SLOT(editClicked()));
  connect(_setButton,SIGNAL(clicked()),this,SLOT(setClicked()));
  connect(_edit1,SIGNAL(valueChanged(int)),this,SLOT(checkChangeCurve()));
  connect(_edit2,SIGNAL(valueChanged(int)),this,SLOT(checkChangeCurve()));
}

void KFEdit2Int::editClicked() {
  AnimationWidget::instance()->addParam(this,_kfX.curve(),QStringList() << name() << "X",_minVal[0],_maxVal[0]);
  AnimationWidget::instance()->addParam(this,_kfY.curve(),QStringList() << name() << "Y",_minVal[1],_maxVal[1]);
  AnimationWidget::instance()->show();
}

void KFEdit2Int::setClicked() {
  float current = (float)AnimationWidget::instance()->currentFrame();
  _kfX.set(_currentVal[0]);
  _kfY.set(_currentVal[1]);
  _kfX.curve()->addKeyframe(Vector2f(current,_kfX.get()));
  _kfY.curve()->addKeyframe(Vector2f(current,_kfY.get()));
  AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "X");
  AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "Y");
}

void KFEdit2Int::checkChangeCurve() {
  if(_kfX.curve()->nbPoints()<2 && _kfY.curve()->nbPoints()<2) {
    const Vector2f p1(_kfX.curve()->point()[0],_currentVal[0]);
    const Vector2f p2(_kfY.curve()->point()[0],_currentVal[1]);
    _kfX.curve()->setKeyframe(p1);
    _kfY.curve()->setKeyframe(p2);
    AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "X");
    AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "Y");
  }
}

bool KFEdit2Int::save(SceneSaver *saver) {
  return (saver->saveIOData("keyframedFloatX",&_kfX) && 
	  saver->saveIOData("keyframedFloatY",&_kfY) && 
	  Edit2Int::save(saver));
}

bool KFEdit2Int::load(SceneLoader *loader) {
  return (loader->loadIOData("keyframedFloatX",&_kfX) && 
	  loader->loadIOData("keyframedFloatY",&_kfY) && 
	  Edit2Int::load(loader));
}



// *****************************************************
// ****************** Edit3Int class *****************
// *****************************************************
Edit3Int::Edit3Int(NodeInterface *node,const QString &name,const Vector3i &minVal,const Vector3i &maxVal,const Vector3i &value)
  : NodeWidget(node),
    _maxVal(maxVal),
    _minVal(minVal),
    _update(true) {
 
  _currentVal = Vector3i(max(min(value[0],maxVal[0]),minVal[0]),
			 max(min(value[1],maxVal[1]),minVal[1]),
			 max(min(value[2],maxVal[2]),minVal[2]));

  _edit1 = new QSpinBox();
  _edit1->setMinimum(minVal[0]);
  _edit1->setMaximum(maxVal[0]);
  _edit1->setValue(_currentVal[0]);

  _edit2 = new QSpinBox();
  _edit2->setMinimum(minVal[1]);
  _edit2->setMaximum(maxVal[1]);
  _edit2->setValue(_currentVal[1]);
 
  _edit3 = new QSpinBox();
  _edit3->setMinimum(minVal[2]);
  _edit3->setMaximum(maxVal[2]);
  _edit3->setValue(_currentVal[2]);

  _nameLabel = new QLabel(name);

  QHBoxLayout *l = new QHBoxLayout();
  l->addWidget(_nameLabel);
  l->addWidget(_edit1);
  l->addWidget(_edit2);
  l->addWidget(_edit3);
  setLayout(l);
  
  connect(_edit1,SIGNAL(valueChanged(int)),this,SLOT(value1Changed(int)));
  connect(_edit2,SIGNAL(valueChanged(int)),this,SLOT(value2Changed(int)));
  connect(_edit3,SIGNAL(valueChanged(int)),this,SLOT(value3Changed(int)));
}

void Edit3Int::value1Changed(int val) {
  _currentVal[0] = val;
  emit valChanged();
  if(_update) {
    updateGraph();
  }
}

void Edit3Int::value2Changed(int val) {
  _currentVal[1] = val;
  emit valChanged();
  if(_update) {
    updateGraph();
  }
}

void Edit3Int::value3Changed(int val) {
  _currentVal[2] = val;
  emit valChanged();
  if(_update) {
    updateGraph();
  }
}

bool Edit3Int::save(SceneSaver *saver) {
  return (saver->saveVector3i("editVal",_currentVal) && 
	  saver->saveVector3i("editMinVal",_minVal) && 
	  saver->saveVector3i("editMaxVal",_maxVal) && 
	  NodeWidget::save(saver));
}

bool Edit3Int::load(SceneLoader *loader) {
  bool ret = (loader->loadVector3i("editVal",_currentVal) && 
	      loader->loadVector3i("editMinVal",_minVal) && 
	      loader->loadVector3i("editMaxVal",_maxVal) && 
	      NodeWidget::load(loader));

  _edit1->setMinimum(_minVal[0]);
  _edit1->setMaximum(_maxVal[0]);
  _edit2->setMinimum(_minVal[1]);
  _edit2->setMaximum(_maxVal[1]);
  _edit3->setMinimum(_minVal[2]);
  _edit3->setMaximum(_maxVal[2]);
  setVal(_currentVal,false);
  return ret;
}



// *****************************************************
// ***************** KFEdit3Int class *****************
// *****************************************************
KFEdit3Int::KFEdit3Int(NodeInterface *node,const QString &name,const Vector3i &minVal,const Vector3i &maxVal,const Vector3i &value)
  : Edit3Int(node,name,minVal,maxVal,value),
    _kfX(minVal[0],maxVal[0],value[0]),
    _kfY(minVal[1],maxVal[1],value[1]),
    _kfZ(minVal[2],maxVal[2],value[2]) {
  
  _editButton = new QPushButton("edit");
  _setButton  = new QPushButton("set");
  layout()->addWidget(_editButton);
  layout()->addWidget(_setButton);

  connect(_editButton,SIGNAL(clicked()),this,SLOT(editClicked()));
  connect(_setButton,SIGNAL(clicked()),this,SLOT(setClicked()));
  connect(_edit1,SIGNAL(valueChanged(int)),this,SLOT(checkChangeCurve()));
  connect(_edit2,SIGNAL(valueChanged(int)),this,SLOT(checkChangeCurve()));
  connect(_edit3,SIGNAL(valueChanged(int)),this,SLOT(checkChangeCurve()));
}

void KFEdit3Int::editClicked() {
  AnimationWidget::instance()->addParam(this,_kfX.curve(),QStringList() << name() << "X",_minVal[0],_maxVal[0]);
  AnimationWidget::instance()->addParam(this,_kfY.curve(),QStringList() << name() << "Y",_minVal[1],_maxVal[1]);
  AnimationWidget::instance()->addParam(this,_kfZ.curve(),QStringList() << name() << "Z",_minVal[2],_maxVal[2]);
  AnimationWidget::instance()->show();
}

void KFEdit3Int::setClicked() {
  float current = (float)AnimationWidget::instance()->currentFrame();
  _kfX.set(_currentVal[0]);
  _kfY.set(_currentVal[1]);
  _kfZ.set(_currentVal[2]);
  _kfX.curve()->addKeyframe(Vector2f(current,_kfX.get()));
  _kfY.curve()->addKeyframe(Vector2f(current,_kfY.get()));
  _kfZ.curve()->addKeyframe(Vector2f(current,_kfZ.get()));
  AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "X");
  AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "Y");
  AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "Z");
}

void KFEdit3Int::checkChangeCurve() {
  if(_kfX.curve()->nbPoints()<2 && _kfY.curve()->nbPoints()<2 && _kfZ.curve()->nbPoints()<2) {
    const Vector2f p1(_kfX.curve()->point()[0],_currentVal[0]);
    const Vector2f p2(_kfY.curve()->point()[0],_currentVal[1]);
    const Vector2f p3(_kfZ.curve()->point()[0],_currentVal[2]);
    _kfX.curve()->setKeyframe(p1);
    _kfY.curve()->setKeyframe(p2);
    _kfZ.curve()->setKeyframe(p3);
    AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "X");
    AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "Y");
    AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "Z");
  }
}


bool KFEdit3Int::save(SceneSaver *saver) {
  return (saver->saveIOData("keyframedFloatX",&_kfX) && 
	  saver->saveIOData("keyframedFloatY",&_kfY) && 
	  saver->saveIOData("keyframedFloatZ",&_kfZ) && 
	  Edit3Int::save(saver));
}

bool KFEdit3Int::load(SceneLoader *loader) {
  return (loader->loadIOData("keyframedFloatX",&_kfX) && 
	  loader->loadIOData("keyframedFloatY",&_kfY) && 
	  loader->loadIOData("keyframedFloatZ",&_kfZ) && 
	  Edit3Int::load(loader));
}




// *****************************************************
// ***************** TrackballWidget class *****************
// *****************************************************
KFTrackballWidget::KFTrackballWidget(NodeInterface *node,const QString &name,TrackballCamera *cam)
  : NodeWidget(node),
    _cam(cam),
    _update(true),
    _kfShiftX(-10000,10000,_cam->shift()[0]),
    _kfShiftY(-10000,10000,_cam->shift()[1]),
    _kfShiftZ(-10000,10000,_cam->shift()[2]),
    _kfQuatX(-10000,10000,_cam->rotation().x()),
    _kfQuatY(-10000,10000,_cam->rotation().y()),
    _kfQuatZ(-10000,10000,_cam->rotation().z()),
    _kfQuatW(-10000,10000,_cam->rotation().w()) {
 

  _projection = new QComboBox();
  _projection->addItems(QStringList() << "perspective" << "orthographic");
  _projection->setCurrentIndex(_cam->projType());
  _rotation = new QComboBox();
  _rotation->addItems(QStringList() << "Scene centered" << "World centered");
  _rotation->setCurrentIndex(_cam->rotType());
  _editButton = new QPushButton("edit");
  _setButton  = new QPushButton("set");
  _nameLabel = new QLabel(name);

  QHBoxLayout *l = new QHBoxLayout();
  l->addWidget(_nameLabel);
  l->addWidget(_projection);
  l->addWidget(_rotation);
  l->addWidget(_editButton);
  l->addWidget(_setButton);
  setLayout(l);

  connect(_projection,SIGNAL(currentIndexChanged(int)),this,SLOT(projectionChanged(int)));
  connect(_rotation,SIGNAL(currentIndexChanged(int)),this,SLOT(rotationChanged(int)));
  connect(_editButton,SIGNAL(clicked()),this,SLOT(editClicked()));
  connect(_setButton,SIGNAL(clicked()),this,SLOT(setClicked()));
  connect(_cam,SIGNAL(cameraChanged()),this,SLOT(checkChangeCurve()));
}

void KFTrackballWidget::projectionChanged(int val) {
  _cam->setProjection(val);
  if(_update) {
    updateGraph();
  }
}

void KFTrackballWidget::rotationChanged(int val) {
  _cam->setRotType(val);
  if(_update) {
    updateGraph();
  }
}

void KFTrackballWidget::checkChangeCurve() {
  if(_kfShiftX.curve()->nbPoints()<2 && _kfShiftY.curve()->nbPoints()<2 && _kfShiftZ.curve()->nbPoints()<2 && 
     _kfQuatX.curve()->nbPoints()<2 && _kfQuatY.curve()->nbPoints()<2 && _kfQuatZ.curve()->nbPoints()<2 && _kfQuatW.curve()->nbPoints()<2) {
    const Vector2f s1(_kfShiftX.curve()->point()[0],_cam->shift()[0]);
    const Vector2f s2(_kfShiftY.curve()->point()[0],_cam->shift()[1]);
    const Vector2f s3(_kfShiftZ.curve()->point()[0],_cam->shift()[2]);
    const Vector2f r1(_kfQuatX.curve()->point()[0],_cam->rotation().x());
    const Vector2f r2(_kfQuatY.curve()->point()[0],_cam->rotation().y());
    const Vector2f r3(_kfQuatZ.curve()->point()[0],_cam->rotation().z());
    const Vector2f r4(_kfQuatW.curve()->point()[0],_cam->rotation().w());

    _kfShiftX.curve()->setKeyframe(s1);
    _kfShiftY.curve()->setKeyframe(s2);
    _kfShiftZ.curve()->setKeyframe(s3);
    _kfQuatX.curve()->setKeyframe(r1);
    _kfQuatY.curve()->setKeyframe(r2);
    _kfQuatZ.curve()->setKeyframe(r3);
    _kfQuatW.curve()->setKeyframe(r4);

    AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "PosX");
    AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "PosY");
    AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "PosZ");
    AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "RotX");
    AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "RotY");
    AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "RotZ");
    AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "RotW");
  }
}

void KFTrackballWidget::editClicked() {
  AnimationWidget::instance()->addParam(this,_kfShiftX.curve(),QStringList() << name() << "PosX",-10000,10000);
  AnimationWidget::instance()->addParam(this,_kfShiftY.curve(),QStringList() << name() << "PosY",-10000,10000);
  AnimationWidget::instance()->addParam(this,_kfShiftZ.curve(),QStringList() << name() << "PosZ",-10000,10000);
  AnimationWidget::instance()->addParam(this,_kfQuatX.curve(),QStringList() << name() << "RotX",-10000,10000);
  AnimationWidget::instance()->addParam(this,_kfQuatY.curve(),QStringList() << name() << "RotY",-10000,10000);
  AnimationWidget::instance()->addParam(this,_kfQuatZ.curve(),QStringList() << name() << "RotZ",-10000,10000);
  AnimationWidget::instance()->addParam(this,_kfQuatW.curve(),QStringList() << name() << "RotW",-10000,10000);
  AnimationWidget::instance()->show();
}

void KFTrackballWidget::setClicked() {
  float current = (float)AnimationWidget::instance()->currentFrame();
  _kfShiftX.set(_cam->shift()[0]);
  _kfShiftY.set(_cam->shift()[1]);
  _kfShiftZ.set(_cam->shift()[2]);
  _kfQuatX.set(_cam->rotation().x());
  _kfQuatY.set(_cam->rotation().y());
  _kfQuatZ.set(_cam->rotation().z());
  _kfQuatW.set(_cam->rotation().w());

  _kfShiftX.curve()->addKeyframe(Vector2f(current,_kfShiftX.get()));
  _kfShiftY.curve()->addKeyframe(Vector2f(current,_kfShiftY.get()));
  _kfShiftZ.curve()->addKeyframe(Vector2f(current,_kfShiftZ.get()));
  _kfQuatX.curve()->addKeyframe(Vector2f(current,_kfQuatX.get()));
  _kfQuatY.curve()->addKeyframe(Vector2f(current,_kfQuatY.get()));
  _kfQuatZ.curve()->addKeyframe(Vector2f(current,_kfQuatZ.get()));
  _kfQuatW.curve()->addKeyframe(Vector2f(current,_kfQuatW.get()));

  AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "PosX");
  AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "PosY");
  AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "PosZ");
  AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "RotX");
  AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "RotY");
  AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "RotZ");
  AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "RotW");
}

bool KFTrackballWidget::save(SceneSaver *saver) {
  return (saver->saveIOData("keyframedFloatPosX",&_kfShiftX) && 
	  saver->saveIOData("keyframedFloatPosY",&_kfShiftY) && 
	  saver->saveIOData("keyframedFloatPosZ",&_kfShiftZ) && 
	  saver->saveIOData("keyframedFloatRotX",&_kfQuatX) && 
	  saver->saveIOData("keyframedFloatRotY",&_kfQuatY) && 
	  saver->saveIOData("keyframedFloatRotZ",&_kfQuatZ) && 
	  saver->saveIOData("keyframedFloatRotW",&_kfQuatW) && 
	  saver->saveInt("projection",_projection->currentIndex()) && 
	  NodeWidget::save(saver));
}

bool KFTrackballWidget::load(SceneLoader *loader) {
  int projection = 0;
  bool ret =  (loader->loadIOData("keyframedFloatPosX",&_kfShiftX) && 
	       loader->loadIOData("keyframedFloatPosY",&_kfShiftY) && 
	       loader->loadIOData("keyframedFloatPosZ",&_kfShiftZ) && 
	       loader->loadIOData("keyframedFloatRotX",&_kfQuatX) && 
	       loader->loadIOData("keyframedFloatRotY",&_kfQuatY) && 
	       loader->loadIOData("keyframedFloatRotZ",&_kfQuatZ) && 
	       loader->loadIOData("keyframedFloatRotW",&_kfQuatW) && 
	       loader->loadInt("projection",projection) && 
	       NodeWidget::load(loader));

  _update = false;
  _projection->setCurrentIndex(projection);
  _update = true;

  return ret;
}






































// *****************************************************
// ****************** ColorPicker class *****************
// *****************************************************
ColorPicker::ColorPicker(NodeInterface *node,const QString &name,const Vector4f &value)
  : NodeWidget(node),
    _update(true) {
 
  _currentVal = Vector4f(max(min(value[0],1.0f),0.0f),
			 max(min(value[1],1.0f),0.0f),
			 max(min(value[2],1.0f),0.0f),
			 max(min(value[3],1.0f),0.0f));


  _colorPicker = new QColorDialog(QColor((int)(_currentVal[0]*255.0f),
					 (int)(_currentVal[1]*255.0f),
					 (int)(_currentVal[2]*255.0f),
					 (int)(_currentVal[3]*255.0f)));
  _colorPicker->setOptions(QColorDialog::ShowAlphaChannel | 
			   QColorDialog::NoButtons | 
			   QColorDialog::DontUseNativeDialog);
  _colorPicker->setWindowFlags(Qt::SubWindow);

  _nameLabel = new QLabel(name);

  QVBoxLayout *l = new QVBoxLayout();
  l->addWidget(_nameLabel);
  l->addWidget(_colorPicker);
  setLayout(l);
  
  connect(_colorPicker,SIGNAL(currentColorChanged(const QColor &)),this,SLOT(colorChanged(const QColor &)));
}

void ColorPicker::colorChanged(const QColor &color) {
  _currentVal = Vector4f((float)(color.red())/255.0f,
			 (float)(color.green())/255.0f,
			 (float)(color.blue())/255.0f,
			 (float)(color.alpha())/255.0f);
  emit valChanged();

  if(_update) {
    updateGraph();
  }
}

bool ColorPicker::save(SceneSaver *saver) {
  return (saver->saveVector4f("color",_currentVal) && 
	  NodeWidget::save(saver));
}

bool ColorPicker::load(SceneLoader *loader) {
  bool ret = (loader->loadVector4f("color",_currentVal) && 
	      NodeWidget::load(loader));
  setVal(_currentVal,false);
  return ret;
}



// *****************************************************
// ***************** KFColorPicker class *****************
// *****************************************************
KFColorPicker::KFColorPicker(NodeInterface *node,const QString &name,const Vector4f &value)
  : ColorPicker(node,name,value),
    _kfR(0,1,value[0]),
    _kfG(0,1,value[1]),
    _kfB(0,1,value[2]),
    _kfA(0,1,value[3]) {
  
  _editButton = new QPushButton("edit");
  _setButton  = new QPushButton("set");

  QHBoxLayout *l = new QHBoxLayout();
  l->addWidget(_editButton);
  l->addWidget(_setButton);
  QWidget *tmp = new QWidget();
  tmp->setLayout(l);
  layout()->addWidget(tmp);
  
  connect(_editButton,SIGNAL(clicked()),this,SLOT(editClicked()));
  connect(_setButton,SIGNAL(clicked()),this,SLOT(setClicked()));
  connect(_colorPicker,SIGNAL(currentColorChanged(const QColor &)),this,SLOT(checkChangeCurve()));
}

void KFColorPicker::editClicked() {
  AnimationWidget::instance()->addParam(this,_kfR.curve(),QStringList() << name() << "R",0,1);
  AnimationWidget::instance()->addParam(this,_kfG.curve(),QStringList() << name() << "G",0,1);
  AnimationWidget::instance()->addParam(this,_kfB.curve(),QStringList() << name() << "B",0,1);
  AnimationWidget::instance()->addParam(this,_kfA.curve(),QStringList() << name() << "A",0,1);
  AnimationWidget::instance()->show();
}

void KFColorPicker::setClicked() {
  float current = (float)AnimationWidget::instance()->currentFrame();
  _kfR.set(_currentVal[0]);
  _kfG.set(_currentVal[1]);
  _kfB.set(_currentVal[2]);
  _kfA.set(_currentVal[3]);

  _kfR.curve()->addKeyframe(Vector2f(current,_kfR.get()));
  _kfG.curve()->addKeyframe(Vector2f(current,_kfG.get()));
  _kfB.curve()->addKeyframe(Vector2f(current,_kfB.get()));
  _kfA.curve()->addKeyframe(Vector2f(current,_kfA.get()));
  AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "R");
  AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "G");
  AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "B");
  AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "A");
}

void KFColorPicker::checkChangeCurve() {
  if(_kfR.curve()->nbPoints()<2 && 
     _kfG.curve()->nbPoints()<2 && 
     _kfB.curve()->nbPoints()<2 && 
     _kfA.curve()->nbPoints()<2) {
    const Vector2f p1(_kfR.curve()->point()[0],_currentVal[0]);
    const Vector2f p2(_kfG.curve()->point()[0],_currentVal[1]);
    const Vector2f p3(_kfB.curve()->point()[0],_currentVal[2]);
    const Vector2f p4(_kfA.curve()->point()[0],_currentVal[3]);
    _kfR.curve()->setKeyframe(p1);
    _kfG.curve()->setKeyframe(p2);
    _kfB.curve()->setKeyframe(p3);
    _kfA.curve()->setKeyframe(p4);
    AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "R");
    AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "G");
    AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "B");
    AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name() << "A");
  }
}

bool KFColorPicker::save(SceneSaver *saver) {
  return (saver->saveIOData("keyframedFloatR",&_kfR) && 
	  saver->saveIOData("keyframedFloatG",&_kfG) && 
	  saver->saveIOData("keyframedFloatB",&_kfB) && 
	  saver->saveIOData("keyframedFloatA",&_kfA) && 
	  ColorPicker::save(saver));
}

bool KFColorPicker::load(SceneLoader *loader) {
  return (loader->loadIOData("keyframedFloatR",&_kfR) && 
	  loader->loadIOData("keyframedFloatG",&_kfG) && 
	  loader->loadIOData("keyframedFloatB",&_kfB) && 
	  loader->loadIOData("keyframedFloatA",&_kfA) && 
	  ColorPicker::load(loader));
}



// *****************************************************
// ****************** ComboBoxInt class ******************
// *****************************************************
ComboBoxInt::ComboBoxInt(NodeInterface *node,const QString &name,const QStringList vals,int value)
  : NodeWidget(node),
    _maxVal(max(vals.size()-1,0)),
    _minVal(0),
    _update(true) {
 
  _currentVal = max(min(value,_maxVal),_minVal);
  _edit = new QComboBox();
  _edit->addItems(vals);
  _edit->setCurrentIndex(value);

  _nameLabel = new QLabel(name);

  QHBoxLayout *l = new QHBoxLayout();
  l->addWidget(_nameLabel);
  l->addWidget(_edit);
  setLayout(l);
  
  connect(_edit,SIGNAL(currentIndexChanged(int)),this,SLOT(valueChanged(int)));
}

void ComboBoxInt::valueChanged(int val) {
  _currentVal = val;
  emit valChanged();
  if(_update) {
    updateGraph();
  }
}

bool ComboBoxInt::save(SceneSaver *saver) {
  return (saver->saveInt("editVal",_currentVal) && 
	  saver->saveInt("editMinVal",_minVal) && 
	  saver->saveInt("editMaxVal",_maxVal) && 
	  NodeWidget::save(saver));
}

bool ComboBoxInt::load(SceneLoader *loader) {
  bool ret = (loader->loadInt("editVal",_currentVal) && 
	      loader->loadInt("editMinVal",_minVal) && 
	      loader->loadInt("editMaxVal",_maxVal) && 
	      NodeWidget::load(loader));

  setVal(_currentVal,false);
  return ret;
}



// *****************************************************
// ***************** KFComboBoxInt class *****************
// *****************************************************
KFComboBoxInt::KFComboBoxInt(NodeInterface *node,const QString &name,const QStringList vals,int value)
  : ComboBoxInt(node,name,vals,value),
    _kf(_minVal,_maxVal,_currentVal) {
  
  _editButton = new QPushButton("edit");
  _setButton  = new QPushButton("set");
  layout()->addWidget(_editButton);
  layout()->addWidget(_setButton);

  connect(_editButton,SIGNAL(clicked()),this,SLOT(editClicked()));
  connect(_setButton,SIGNAL(clicked()),this,SLOT(setClicked()));
  connect(_edit,SIGNAL(currentIndexChanged(int)),this,SLOT(checkChangeCurve()));
}

void KFComboBoxInt::editClicked() {
  AnimationWidget::instance()->addParam(this,_kf.curve(),QStringList() << name(),_minVal,_maxVal);
  AnimationWidget::instance()->show();
}

void KFComboBoxInt::setClicked() {
  float current = (float)AnimationWidget::instance()->currentFrame();
  _kf.set(_currentVal);
  _kf.curve()->addKeyframe(Vector2f(current,_kf.get()));
  AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name());
}

void KFComboBoxInt::checkChangeCurve() {
  if(_kf.curve()->nbPoints()<2) {
    const Vector2f p(_kf.curve()->point()[0],_currentVal);
    _kf.curve()->setKeyframe(p);
    AnimationWidget::instance()->controlPointsChanged(this,QStringList() << name());
  }
}

bool KFComboBoxInt::save(SceneSaver *saver) {
  return saver->saveIOData("keyframedFloat",&_kf) && ComboBoxInt::save(saver);
}

bool KFComboBoxInt::load(SceneLoader *loader) {
  return loader->loadIOData("keyframedFloat",&_kf) && ComboBoxInt::load(loader);
}

