// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "genericCustomWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QScrollArea>
#include <QMessageBox>
#include <QDebug>

using namespace std;

GenericCustomIOItemButtons::GenericCustomIOItemButtons()
  : _up(new QToolButton()),
    _down(new QToolButton()),
    _del(new QToolButton()) {
  _up->setText("Up");
  _down->setText("Down");
  _del->setText("Remove");
  
  QHBoxLayout *l = new QHBoxLayout();
  l->addWidget(_up);
  l->addWidget(_down);
  l->addWidget(_del);
  setLayout(l);
}
    
GenericCustomIOItem::GenericCustomIOItem(unsigned int id,const QString &name) 
  : _id(id),
    _number(new QLabel(QString::number(_id))),
    _name(new QLineEdit(name)),
    _buttons(new GenericCustomIOItemButtons()) {

  QGridLayout *l = new QGridLayout();
  
  l->addWidget(_number,0,0);
  l->addWidget(_name,0,1);
  l->addWidget(_buttons,0,2);

  setLayout(l);
  
  connect(_buttons->up(),SIGNAL(clicked()),this,SLOT(needUpSignal()));
  connect(_buttons->down(),SIGNAL(clicked()),this,SLOT(needDownSignal()));
  connect(_buttons->del(),SIGNAL(clicked()),this,SLOT(needDelSignal()));
}

void GenericCustomIOItem::needUpSignal  () { emit upClicked(this);   }
void GenericCustomIOItem::needDownSignal() { emit downClicked(this); }
void GenericCustomIOItem::needDelSignal () { emit delClicked(this);  }



const QStringList GenericCustomParamItem::TYPES = QStringList() << "sliderF" << "sliderI";

GenericCustomParamItem::GenericCustomParamItem(unsigned int id,const QString &name,int type,bool kf,float minVal,float maxVal,float defaultVal) 
  : _id(id),
    _name(new QLineEdit(name)),
    _minEdit(new QLineEdit(QString::number(minVal))),
    _maxEdit(new QLineEdit(QString::number(maxVal))),
    _valEdit(new QLineEdit(QString::number(defaultVal))),
    _minLabel(new QLabel("Min:")),
    _maxLabel(new QLabel("Max:")),
    _valLabel(new QLabel("Default:")),
    _type(new QComboBox()),
    _kf(new QCheckBox("Keyframes?")),
    _del(new QToolButton()) {

  _type->addItems(TYPES);
  _type->setCurrentIndex(type);
  _del->setText("Remove");
  _kf->setCheckState(kf ? Qt::Checked : Qt::Unchecked);

  QGridLayout *l = new QGridLayout();
  
  l->addWidget(_name,0,0,1,1);
  l->addWidget(_type,0,1,1,2);
  l->addWidget(_kf  ,0,3,1,2);
  l->addWidget(_del ,0,5,1,1);

  l->addWidget(_minLabel,1,0);
  l->addWidget(_minEdit ,1,1);
  l->addWidget(_maxLabel,1,2);
  l->addWidget(_maxEdit ,1,3);
  l->addWidget(_valLabel,1,4);
  l->addWidget(_valEdit ,1,5);

  QFrame *line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  l->addWidget(line,2,0,1,5);
  setLayout(l);
  
  connect(_del,SIGNAL(clicked()),this,SLOT(needDelSignal()));
}

void GenericCustomParamItem::needDelSignal () { emit delClicked(this);  }


GenericCustomWidgetSetting::GenericCustomWidgetSetting(GenericCustomWidget *nodeWidget,bool needMouseWidget)
  : _nodeWidget(nodeWidget),
    _inputBox(new QGroupBox("Input data")),
    _addInput(new QPushButton("Add input")),
    _inputLayout(new QVBoxLayout()),
    _outputBox(new QGroupBox("Output data")),
    _addOutput(new QPushButton("Add output")),
    _outputLayout(new QVBoxLayout()),
    _paramBox(new QGroupBox("Param data")),
    _addParam(new QPushButton("Add param")),
    _paramLayout(new QVBoxLayout()),
    _title(new QLabel("<center>Node parameters<br>Warning: this action will remove previously defined keyframes</center>")),
    _specialBox(new QGroupBox("Special data")),
    _xSize(new QLineEdit()),
    _ySize(new QLineEdit()),
    _xOffset(new QLineEdit()),
    _yOffset(new QLineEdit()),
    _buttons(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel)) {

  _useMouse = needMouseWidget ? new QCheckBox("use mouse?") : NULL;

  initInputItems();
  buildInputLayout();
  initOutputItems();
  buildOutputLayout();
  initParamItems();
  buildParamLayout();

  const Vector2f tsize   = _nodeWidget->tsize();
  const Vector2f toffset = _nodeWidget->toffset();

  _xSize->setText(QString::number(tsize[0]));
  _ySize->setText(QString::number(tsize[1]));
  _xOffset->setText(QString::number(toffset[0]));
  _yOffset->setText(QString::number(toffset[1]));
  if(_useMouse)
    _useMouse->setCheckState(_nodeWidget->useMouse() ? Qt::Checked : Qt::Unchecked);

  QVBoxLayout *mainLayout = new QVBoxLayout();
  QVBoxLayout *l;
  QWidget     *tmp;

  QGridLayout *g = new QGridLayout();
  g->addWidget(new QLabel("Out size = "),0,0);
  g->addWidget(_xSize,0,1);
  g->addWidget(_ySize,0,2);
  g->addWidget(new QLabel("* In size "),0,3);
  g->addWidget(new QLabel(" + "),1,0);
  g->addWidget(_xOffset,1,1);
  g->addWidget(_yOffset,1,2);
  if(_useMouse) g->addWidget(_useMouse,2,0,1,3);
  _specialBox->setLayout(g);

  QScrollArea *inputArea = new QScrollArea();
  inputArea->setWidgetResizable(true);
  tmp = new QWidget();
  tmp->setLayout(_inputLayout);
  inputArea->setWidget(tmp);
  l = new QVBoxLayout();
  l->addWidget(inputArea);
  l->addWidget(_addInput);
  _inputBox->setLayout(l);

  QScrollArea *outputArea = new QScrollArea();
  outputArea->setWidgetResizable(true);
  tmp = new QWidget();
  tmp->setLayout(_outputLayout);
  outputArea->setWidget(tmp);
  l = new QVBoxLayout();
  l->addWidget(outputArea);
  l->addWidget(_addOutput);
  _outputBox->setLayout(l);

  QScrollArea *paramArea = new QScrollArea();
  paramArea->setWidgetResizable(true);
  tmp = new QWidget();
  tmp->setLayout(_paramLayout);
  paramArea->setWidget(tmp);
  l = new QVBoxLayout();
  l->addWidget(paramArea);
  l->addWidget(_addParam);
  _paramBox->setLayout(l);

  mainLayout->addWidget(_title);
  mainLayout->addWidget(_specialBox);
  mainLayout->addWidget(_inputBox);
  mainLayout->addWidget(_outputBox);
  mainLayout->addWidget(_paramBox);
  mainLayout->addWidget(_buttons);

  setLayout(mainLayout);

  connect(_buttons,SIGNAL(accepted()),this,SLOT(confirmClicked()));
  connect(_buttons,SIGNAL(rejected()),this,SLOT(cancelClicked()));
  connect(_addInput,SIGNAL(clicked()),this,SLOT(addInputClicked()));
  connect(_addOutput,SIGNAL(clicked()),this,SLOT(addOutputClicked()));
  connect(_addParam,SIGNAL(clicked()),this,SLOT(addParamClicked()));

  resize(600,800);
}

GenericCustomWidgetSetting::~GenericCustomWidgetSetting() {
  cleanInputLayout();
  cleanInputItems();
  cleanOutputLayout();
  cleanOutputItems();
  cleanParamLayout();
  cleanParamItems();
}

void GenericCustomWidgetSetting::inputItemUp(GenericCustomIOItem *item) {
  if(item->id()==0) return;
  const unsigned int id = item->id();
  GenericCustomIOItem *tmpItem = _inputItems[id-1];
  _inputItems[id-1]->setId(id);
  _inputItems[id]->setId(id-1);
  _inputItems[id-1] = item;
  _inputItems[id] = tmpItem;
  cleanInputLayout();
  buildInputLayout();
}

void GenericCustomWidgetSetting::inputItemDown(GenericCustomIOItem *item) {
  if(item->id()==_inputItems.size()-1) return;
  const unsigned int id = item->id();
  GenericCustomIOItem *tmpItem = _inputItems[id+1];
  _inputItems[id+1]->setId(id);
  _inputItems[id]->setId(id+1);
  _inputItems[id+1] = item;
  _inputItems[id] = tmpItem;
  cleanInputLayout();
  buildInputLayout();
}

void GenericCustomWidgetSetting::inputItemDel(GenericCustomIOItem *item) {
  if(_nodeWidget->nodePartiallyConnected()) {
    showErrorMessage("Unable to remove input\nPlease, remove all connections from the node first");
    return;
  }

  cleanInputLayout();

  unsigned int id = item->id();
  delete item;
  _inputItems.erase(_inputItems.begin()+id);
  
  for(unsigned int i=id;i<_inputItems.size();++i) {
    _inputItems[i]->setId(_inputItems[i]->id()-1); 
  }

  buildInputLayout();
}
  
void GenericCustomWidgetSetting::addInputClicked() {
  if(_nodeWidget->nodePartiallyConnected()) {
    showErrorMessage("Unable to add input\nPlease, remove all connections from the node first");
    return;
  }

  unsigned int index = _inputItems.size();
  _inputItems.push_back(new GenericCustomIOItem(index,"inBuffer"+QString::number(index)));
  connect(_inputItems[index],SIGNAL(upClicked(GenericCustomIOItem *)),this,SLOT(inputItemUp(GenericCustomIOItem *)));
  connect(_inputItems[index],SIGNAL(downClicked(GenericCustomIOItem *)),this,SLOT(inputItemDown(GenericCustomIOItem *)));
  connect(_inputItems[index],SIGNAL(delClicked(GenericCustomIOItem *)),this,SLOT(inputItemDel(GenericCustomIOItem *)));
  _inputLayout->addWidget(_inputItems[index]);
}
  
void GenericCustomWidgetSetting::buildInputLayout() {
  for(unsigned int i=0;i<_inputItems.size();++i) {
    _inputLayout->addWidget(_inputItems[i]);
  }
}

void GenericCustomWidgetSetting::cleanInputLayout() {
  for(unsigned int i=0;i<_inputItems.size();++i) {
    _inputLayout->removeWidget(_inputItems[i]);
  }
}

void GenericCustomWidgetSetting::initInputItems() {
  vector<QString> names = _nodeWidget->inputNames();
  for(unsigned int i=0;i<names.size();++i) {
    _inputItems.push_back(new GenericCustomIOItem(i,names[i]));
    connect(_inputItems[i],SIGNAL(upClicked(GenericCustomIOItem *)),this,SLOT(inputItemUp(GenericCustomIOItem *)));
    connect(_inputItems[i],SIGNAL(downClicked(GenericCustomIOItem *)),this,SLOT(inputItemDown(GenericCustomIOItem *)));
    connect(_inputItems[i],SIGNAL(delClicked(GenericCustomIOItem *)),this,SLOT(inputItemDel(GenericCustomIOItem *)));
  }
}

void GenericCustomWidgetSetting::cleanInputItems() {
  for(unsigned int i=0;i<_inputItems.size();++i) {
    delete _inputItems[i];
  }
  _inputItems.clear();
}

void GenericCustomWidgetSetting::outputItemUp(GenericCustomIOItem *item) {
  if(item->id()==0) return;
  const unsigned int id = item->id();
  GenericCustomIOItem *tmpItem = _outputItems[id-1];
  _outputItems[id-1]->setId(id);
  _outputItems[id]->setId(id-1);
  _outputItems[id-1] = item;
  _outputItems[id] = tmpItem;
  cleanOutputLayout();
  buildOutputLayout();
}

void GenericCustomWidgetSetting::outputItemDown(GenericCustomIOItem *item) {
  if(item->id()==_outputItems.size()-1) return;
  const unsigned int id = item->id();
  GenericCustomIOItem *tmpItem = _outputItems[id+1];
  _outputItems[id+1]->setId(id);
  _outputItems[id]->setId(id+1);
  _outputItems[id+1] = item;
  _outputItems[id] = tmpItem;
  cleanOutputLayout();
  buildOutputLayout();
}

void GenericCustomWidgetSetting::outputItemDel(GenericCustomIOItem *item) {
  if(_nodeWidget->nodePartiallyConnected()) {
    showErrorMessage("Unable to remove output\nPlease, remove all connections from the node first");
    return;
  }

  cleanOutputLayout();

  unsigned int id = item->id();
  delete item;
  _outputItems.erase(_outputItems.begin()+id);
  
  for(unsigned int i=id;i<_outputItems.size();++i) {
    _outputItems[i]->setId(_outputItems[i]->id()-1); 
  }

  buildOutputLayout();
}
  
void GenericCustomWidgetSetting::addOutputClicked() {
  if(_nodeWidget->nodePartiallyConnected()) {
    showErrorMessage("Unable to add output\nPlease, remove all connections from the node first");
    return;
  }

  unsigned int index = _outputItems.size();
  _outputItems.push_back(new GenericCustomIOItem(index,"outBuffer"+QString::number(index)));
  connect(_outputItems[index],SIGNAL(upClicked(GenericCustomIOItem *)),this,SLOT(outputItemUp(GenericCustomIOItem *)));
  connect(_outputItems[index],SIGNAL(downClicked(GenericCustomIOItem *)),this,SLOT(outputItemDown(GenericCustomIOItem *)));
  connect(_outputItems[index],SIGNAL(delClicked(GenericCustomIOItem *)),this,SLOT(outputItemDel(GenericCustomIOItem *)));
  _outputLayout->addWidget(_outputItems[index]);
}
  
void GenericCustomWidgetSetting::buildOutputLayout() {
  for(unsigned int i=0;i<_outputItems.size();++i) {
    _outputLayout->addWidget(_outputItems[i]);
  }
}

void GenericCustomWidgetSetting::cleanOutputLayout() {
  for(unsigned int i=0;i<_outputItems.size();++i) {
    _outputLayout->removeWidget(_outputItems[i]);
  }
}

void GenericCustomWidgetSetting::initOutputItems() {
  vector<QString> names = _nodeWidget->outputNames();
  for(unsigned int i=0;i<names.size();++i) {
    _outputItems.push_back(new GenericCustomIOItem(i,names[i]));
    connect(_outputItems[i],SIGNAL(upClicked(GenericCustomIOItem *)),this,SLOT(outputItemUp(GenericCustomIOItem *)));
    connect(_outputItems[i],SIGNAL(downClicked(GenericCustomIOItem *)),this,SLOT(outputItemDown(GenericCustomIOItem *)));
    connect(_outputItems[i],SIGNAL(delClicked(GenericCustomIOItem *)),this,SLOT(outputItemDel(GenericCustomIOItem *)));
  }
}

void GenericCustomWidgetSetting::cleanOutputItems() {
  for(unsigned int i=0;i<_outputItems.size();++i) {
    delete _outputItems[i];
  }
  _outputItems.clear();
}


void GenericCustomWidgetSetting::paramItemDel(GenericCustomParamItem *item) {
  cleanParamLayout();

  unsigned int id = item->id();
  delete item;
  _paramItems.erase(_paramItems.begin()+id);
  
  for(unsigned int i=id;i<_paramItems.size();++i) {
    _paramItems[i]->setId(_paramItems[i]->id()-1); 
  }

  buildParamLayout();
}
  
void GenericCustomWidgetSetting::addParamClicked() {
  unsigned int index = _paramItems.size();
  _paramItems.push_back(new GenericCustomParamItem(index,"param"+QString::number(index)));
  connect(_paramItems[index],SIGNAL(delClicked(GenericCustomParamItem *)),this,SLOT(paramItemDel(GenericCustomParamItem *)));
  _paramLayout->addWidget(_paramItems[index]);
}
  
void GenericCustomWidgetSetting::buildParamLayout() {
  for(unsigned int i=0;i<_paramItems.size();++i) {
    _paramLayout->addWidget(_paramItems[i]);
  }
}

void GenericCustomWidgetSetting::cleanParamLayout() {
  for(unsigned int i=0;i<_paramItems.size();++i) {
    _paramLayout->removeWidget(_paramItems[i]);
  }
}

void GenericCustomWidgetSetting::initParamItems() {

  vector<SliderFloat *> fs = _nodeWidget->floatSliders();
  for(unsigned int i=0;i<fs.size();++i) {
    const float minVal = (float)fs[i]->minVal();
    const float maxVal = (float)fs[i]->maxVal();
    const float defVal = (float)fs[i]->val();
    const int   type   = 0;
    const bool  kf     = fs[i]->isKeyframed();
    _paramItems.push_back(new GenericCustomParamItem(i,fs[i]->name(),type,kf,minVal,maxVal,defVal));
    connect(_paramItems[i],SIGNAL(delClicked(GenericCustomParamItem *)),this,SLOT(paramItemDel(GenericCustomParamItem *)));
  }

  vector<SliderInt *> is = _nodeWidget->intSliders();
  for(unsigned int i=0;i<is.size();++i) {
    const float minVal = (float)is[i]->minVal();
    const float maxVal = (float)is[i]->maxVal();
    const float defVal = (float)is[i]->val();
    const int   type   = 1;
    const bool  kf     = is[i]->isKeyframed();
    _paramItems.push_back(new GenericCustomParamItem(i,is[i]->name(),type,kf,minVal,maxVal,defVal));
    connect(_paramItems[i],SIGNAL(delClicked(GenericCustomParamItem *)),this,SLOT(paramItemDel(GenericCustomParamItem *)));
  }
}

void GenericCustomWidgetSetting::cleanParamItems() {
  for(unsigned int i=0;i<_paramItems.size();++i) {
    delete _paramItems[i];
  }
  _paramItems.clear();
}

std::vector<GenericCustomIOItem *> GenericCustomWidgetSetting::inputItems() const {
  return _inputItems;
}

std::vector<GenericCustomIOItem *> GenericCustomWidgetSetting::outputItems() const {
  return _outputItems;
}

std::vector<GenericCustomParamItem *> GenericCustomWidgetSetting::paramItems() const {
  return _paramItems;
}

const Vector2f GenericCustomWidgetSetting::tsize(bool *ok) const {
  Vector2f tsize;
  tsize[0]   = _xSize->text().toFloat(ok);
  tsize[1]   = _ySize->text().toFloat(ok);
  return tsize;
}

const Vector2f GenericCustomWidgetSetting::toffset(bool *ok) const {
  Vector2f toffset;
  toffset[0] = _xOffset->text().toFloat(ok);
  toffset[1] = _yOffset->text().toFloat(ok);
  return toffset;
}

bool GenericCustomWidgetSetting::useMouse() const {
  return _useMouse ? _useMouse->checkState()==Qt::Checked : false;
}

bool GenericCustomWidgetSetting::checkParams() {
  // check if the output size is null
  bool ok = true;
  Vector2f ts = tsize(&ok);
  Vector2f to = toffset(&ok);

  if(!ok) {
    showErrorMessage("Output size must contain valid numbers.");
    return false;
  }

  if((_inputItems.empty() || ts==Vector2f(0,0)) && to==Vector2f(0,0)) {
    showErrorMessage("Output size must not be zero");
    return false;
  }

  if(_outputItems.empty()) {
    showErrorMessage("At least one output image needed");
    return false;
  }

  QStringList names;
  for(unsigned int i=0;i<_inputItems.size();++i) {
    names.push_back(_inputItems[i]->name());
  }

  for(unsigned int i=0;i<_outputItems.size();++i) {
    names.push_back(_outputItems[i]->name());
  }

  for(unsigned int i=0;i<_paramItems.size();++i) {
    names.push_back(_paramItems[i]->name());
  }

  if(names.removeDuplicates()>0) {
    showErrorMessage("At least two parameters/inputs/outputs have the same name");
    return false;
  }

  for(int i=0;i<names.size();++i) {
    // non exhaustive list of special caracters
    if(names[i].contains(' ')) {
      showErrorMessage("Names must not contain spaces");
      return false;
    }
  }

  if(_outputItems.empty()) {
    showErrorMessage("The node should contain at least one output");
    return false;
  }

  return true;
}

void GenericCustomWidgetSetting::showErrorMessage(const QString &msg) {
  QMessageBox::information(this,tr("Error"),msg);
}

void GenericCustomWidgetSetting::confirmClicked() {
  if(!checkParams()) return;

  accept();
}

void GenericCustomWidgetSetting::cancelClicked() {
  reject();
}




GenericCustomWidget::GenericCustomWidget(GenericCustomNode *node)
  : NodeWidget(node),
    _customNode(node),
    _userLayout(new QVBoxLayout()),
    _mouseLabel(NULL) {

  

}


QString GenericCustomWidget::generateHead(const QString &b,bool addOutputLocations) {
  QString head = b;

  if(addOutputLocations) {
    const std::vector<QString> onames = outputNames();
    for(unsigned int i=0;i<onames.size();++i) {
      head = head+tr("layout(location = ")+QString::number(i)+tr(") out vec4 ")+onames[i]+tr(";\n");
    }
  }

  const std::vector<QString> inames = inputNames();
  for(unsigned int i=0;i<inames.size();++i) {
    head = head+tr("uniform sampler2D ")+inames[i]+tr(";\n");
  }
  
  if(useMouse()) {
    head = head+tr("uniform vec2 ")+_mouseLabel->name()+tr(";\n");
  }

  for(unsigned int i=0;i<_floatSliders.size();++i) {
    head = head+tr("uniform float ")+_floatSliders[i]->name()+tr(";\n");
  }

  for(unsigned int i=0;i<_intSliders.size();++i) {
    head = head+tr("uniform int ")+_intSliders[i]->name()+tr(";\n");
  }

  return head;
}

void GenericCustomWidget::addUniformsToProg(GPUProgram *p) {
  const QStringList uniforms = getUniformNames();
  for(int i=0;i<uniforms.size();++i) {
    p->addUniform(uniforms[i].toStdString());
  }
}

void GenericCustomWidget::sendUniformsToProg(GPUProgram *p) {

  if(useMouse()) {
    Vector2f v = _mouseLabel->val();
    p->setUniform2fv(_mouseLabel->name().toStdString(),v.data());
  }

  for(unsigned int i=0;i<_floatSliders.size();++i) {
    p->setUniform1f(_floatSliders[i]->name().toStdString(),_floatSliders[i]->val());
  }

  for(unsigned int i=0;i<_intSliders.size();++i) {
    p->setUniform1i(_intSliders[i]->name().toStdString(),_intSliders[i]->val());
  }
}

const QStringList GenericCustomWidget::getUniformNames() {
  QStringList l;

  const std::vector<QString> inames = inputNames();

  for(unsigned int i=0;i<inames.size();++i) {
    l.push_back(inames[i]);
  }

  if(useMouse()) {
    l.push_back(_mouseLabel->name());
  }

  for(unsigned int i=0;i<_floatSliders.size();++i) {
    l.push_back(_floatSliders[i]->name());
  }

  for(unsigned int i=0;i<_intSliders.size();++i) {
    l.push_back(_intSliders[i]->name());
  }
  return l;
}

const std::vector<QString> GenericCustomWidget::inputNames() {
  unsigned int nb = _customNode->nbInputs();
  QStringList l = _customNode->inputDesc();

  vector<QString> names;
  for(unsigned i=0;i<nb;++i) {
    QString current = (int)i<l.size() ? l[i] : "inBuffer"+QString::number(i);
    current.remove(QChar(' ')); // remove spaces (just in case)
    names.push_back(current);
  }

  return names;
}

const std::vector<QString> GenericCustomWidget::outputNames() {
  unsigned int nb = _customNode->nbOutputs();
  QStringList l = _customNode->outputDesc();

  vector<QString> names;
  for(unsigned i=0;i<nb;++i) {
    QString current = (int)i<l.size() ? l[i] : "outBuffer"+QString::number(i);
    current.remove(QChar(' ')); // remove spaces (just in case)
    names.push_back(current);
  }
  
  return names;
}

bool GenericCustomWidget::nodePartiallyConnected() const {
  return _customNode->isPartlyConnected();
}

Vector2f GenericCustomWidget::tsize  () const {
  return _customNode->tsize();
}

Vector2f GenericCustomWidget::toffset() const {
  return _customNode->toffset();
}


void GenericCustomWidget::cleanLayout() {
  if(useMouse()) 
    _userLayout->removeWidget(_mouseLabel);

  for(unsigned int i=0;i<_floatSliders.size();++i) {
    _userLayout->removeWidget(_floatSliders[i]);
  }

  for(unsigned int i=0;i<_intSliders.size();++i) {
    _userLayout->removeWidget(_intSliders[i]);
  }
}

void GenericCustomWidget::initLayout() {
  if(useMouse()) {
    _userLayout->addWidget(_mouseLabel);
  }

  for(unsigned int i=0;i<_floatSliders.size();++i) {
    _userLayout->addWidget(_floatSliders[i]);
  }

  for(unsigned int i=0;i<_intSliders.size();++i) {
    _userLayout->addWidget(_intSliders[i]);
  }
}

void GenericCustomWidget::cleanWidgets() {

  // mouse 
  delChildWidget(_mouseLabel);
  delete _mouseLabel;
  _mouseLabel = NULL;

  // slider floats
  for(unsigned int i=0;i<_floatSliders.size();++i) {
    delChildWidget(_floatSliders[i]);
    delete _floatSliders[i];
  }

  // slider ints
  for(unsigned int i=0;i<_intSliders.size();++i) {
    delChildWidget(_intSliders[i]);
    delete _intSliders[i];
  }

  _floatSliders.clear();
  _intSliders.clear();
}



void GenericCustomWidget::initFromSettings(const GenericCustomWidgetSetting &settings) {

  // check if the number of inputs/outputs has changed
  std::vector<GenericCustomIOItem *> iitems = settings.inputItems();
  std::vector<GenericCustomIOItem *> oitems = settings.outputItems();
  const unsigned int nbIn  = iitems.size();
  const unsigned int nbOut = oitems.size();
  if(!nodePartiallyConnected() && (nbIn !=_customNode->nbInputs() || 
				   nbOut!=_customNode->nbOutputs())) {
    // we have to change the number of inputs/outputs
    _customNode->setIONumbers(nbIn,nbOut);
  }

  // set input/output names 
  QStringList inames;
  QStringList onames;
  for(unsigned int i=0;i<nbIn;++i) {
    inames.push_back(iitems[i]->name());
  }
  for(unsigned int i=0;i<nbOut;++i) {
    onames.push_back(oitems[i]->name());
  }
  _customNode->setIONames(inames,onames);

  // set outputsize 
  const Vector2f ts = tsize();
  const Vector2f to = toffset();
  if(ts!=settings.tsize() || to!=settings.toffset()) {
    _customNode->setImgSize(settings.tsize(),settings.toffset());    
  }

  initWidgets(settings);
  initLayout();
}


void GenericCustomWidget::initWidgets(const GenericCustomWidgetSetting &settings) {
  // clean everything 
  cleanLayout();
  cleanWidgets();

  // mouse widget 
  if(settings.useMouse()) {
    _mouseLabel = new KFLabel2Float(_customNode,"mousePos");
    addChildWidget(_mouseLabel);
  }

  // user-defined widgets 
  std::vector<GenericCustomParamItem *> params = settings.paramItems();
  for(unsigned int i=0;i<params.size();++i) {
    // get all params 
    const int type     = params[i]->type();
    const bool kf      = params[i]->isKF();
    const QString name = params[i]->name();
    const float minVal = params[i]->minVal();
    const float maxVal = params[i]->maxVal();
    const float val    = params[i]->val();

    switch(type) {
    case 0: // float slider
      _floatSliders.push_back(kf ? 
			      new KFSliderFloat(_customNode,name,minVal,maxVal,val) : 
			      new SliderFloat(_customNode,name,minVal,maxVal,val));
      addChildWidget(_floatSliders[_floatSliders.size()-1]);
      break;
    case 1: // int slider
      _intSliders.push_back(kf ? 
			    new KFSliderInt(_customNode,name,(int)minVal,(int)maxVal,(int)val) : 
			    new SliderInt(_customNode,name,(int)minVal,(int)maxVal,(int)val));
      addChildWidget(_intSliders[_intSliders.size()-1]);
      break;

    default:break;
    }
  }
}

bool GenericCustomWidget::saveWidgets(SceneSaver  *saver) {
  // we only have to save/load widget names
  // they will be automatically fully loaded by the nodeWidget class
  bool ret = true;

  // mouse 
  ret = ret && saver->saveBool("genericNeedMouse",useMouse());

  // float sliders
  ret = ret && saver->saveUint("genericNbSliderFloat",_floatSliders.size());
  for(unsigned int i=0;i<_floatSliders.size();++i) {
    ret = ret && saver->saveBool(Tags::attribId("genericSliderFloatIsKF",i),_floatSliders[i]->isKeyframed());
    ret = ret && saver->saveString(Tags::attribId("genericSliderFloatName",i),_floatSliders[i]->name());
  }

  // int sliders
  ret = ret && saver->saveUint("genericNbSliderInt",_intSliders.size());
  for(unsigned int i=0;i<_intSliders.size();++i) {
    ret = ret && saver->saveBool(Tags::attribId("genericSliderIntIsKF",i),_intSliders[i]->isKeyframed());
    ret = ret && saver->saveString(Tags::attribId("genericSliderIntName",i),_intSliders[i]->name());
  }
  return ret;
}

bool GenericCustomWidget::loadWidgets(SceneLoader *loader) {
  // we only have to save/load widget names
  // they will be automatically fully loaded by the nodeWidget class
  bool ret = true;

  cleanLayout();
  cleanWidgets();

  bool usemouse = false;
  unsigned int nb;

  // mouse 
  ret = ret && loader->loadBool("genericNeedMouse",usemouse);
  if(usemouse) {
    _mouseLabel = new KFLabel2Float(_customNode,"mousePos");
    addChildWidget(_mouseLabel);
  }

  // float sliders 
  nb = 0;
  ret = ret && loader->loadUint("genericNbSliderFloat",nb);
  for(unsigned int i=0;i<nb;++i) {
    bool    kf   = false;
    QString name = "sliderF"+QString::number(i);
    ret = ret && loader->loadBool(Tags::attribId("genericSliderFloatIsKF",i),kf);
    ret = ret && loader->loadString(Tags::attribId("genericSliderFloatName",i),name);
    _floatSliders.push_back(kf ? new KFSliderFloat(_customNode,name) : new SliderFloat(_customNode,name));
    addChildWidget(_floatSliders[i]);
  }

  // int sliders 
  nb = 0;
  ret = ret && loader->loadUint("genericNbSliderInt",nb);
  for(unsigned int i=0;i<nb;++i) {
    bool    kf   = false;
    QString name = "sliderI"+QString::number(i);
    ret = ret && loader->loadBool(Tags::attribId("genericSliderIntIsKF",i),kf);
    ret = ret && loader->loadString(Tags::attribId("genericSliderIntName",i),name);
    _intSliders.push_back(kf ? new KFSliderInt(_customNode,name) : new SliderInt(_customNode,name));
    addChildWidget(_intSliders[i]);
  }

  return ret;
}

bool GenericCustomWidget::loadUserDefinedWidgets(SceneLoader *) {
  initLayout();
  return true;
}



GenericCustomNode::GenericCustomNode(PbGraph *parent,NodeHandle *handle,unsigned int nbInputs,unsigned int nbOutputs)
  : NodeTexture2D(parent,handle),
    _nbInputs(nbInputs),
    _nbOutputs(nbOutputs),
    _tsize(1,1),
    _toffset(0,0) {
}

bool GenericCustomNode::save(SceneSaver  *saver) {
  bool ret = true;

  // save input names (they are not constant)
  const QStringList inames = inputDesc();
  for(int i=0;i<inames.size();++i) {
    if(!saver->saveString(Tags::attribId("genericIName",i),inames[i])) ret = false;
  }

  // save output names as well
  const QStringList onames = outputDesc();
  for(int i=0;i<onames.size();++i) {
    if(!saver->saveString(Tags::attribId("genericOName",i),onames[i])) ret = false;
  }

  // save output size 
  if(!saver->saveVector2f("genericTSize",_tsize))     ret = false;
  if(!saver->saveVector2f("genericTOffset",_toffset)) ret = false;

  // widgets 
  if(!((GenericCustomWidget *)widget())->saveWidgets(saver)) ret = false;

  return ret;
}

bool GenericCustomNode::load(SceneLoader *loader) {

  // the number of inputs/outputs should have already been set in thanks to the constructor
  bool ret = true;

  // load input names 
  QStringList inames;
  for(int i=0;i<(int)nbInputs();++i) {
    QString name = "inBuffer"+QString::number(i);
    if(!loader->loadString(Tags::attribId("genericIName",i),name)) ret = false;
    inames.push_back(name);
  }

  // save output names as well
  QStringList onames;
  for(int i=0;i<(int)nbOutputs();++i) {
    QString name = "outBuffer"+QString::number(i);
    if(!loader->loadString(Tags::attribId("genericOName",i),name)) ret = false;
    onames.push_back(name);
  }

  // set names 
  setIONames(inames,onames);

  // load output size 
  if(!loader->loadVector2f("genericTSize",_tsize))     ret = false;
  if(!loader->loadVector2f("genericTOffset",_toffset)) ret = false;
  setImgSize(_tsize,_toffset);

  // create widgets 
  if(!((GenericCustomWidget *)widget())->loadWidgets(loader)) ret = false;
  
  return ret;
}



void GenericCustomNode::setImgSize(const Vector2f &tsize,const Vector2f &toffset) {
  _tsize = tsize;
  _toffset = toffset;

  if(isReady() && !_inputData.empty() && input(0)->type()==Data::TEX2D) {
    Vector2f texSize(((TextureData *)input(0))->tex()->w()*_tsize[0],
		     ((TextureData *)input(0))->tex()->h()*_tsize[1]);
    setOutputSize(texSize+_toffset);
  } else if(_inputData.empty() && _toffset!=Vector2f(0,0)) {
    setOutputSize(_toffset);
  }
}

void GenericCustomNode::initOutputData()   {
  if(!checkInputData(inputs())) {
    qDebug() << "Warning: trying to init output data while input not ready!" << endl;
    return;
  }

  // clean everything 
  cleanOutputData();

  //Vector2f size = outputContentSize(0);
  Vector2f size = outputSize();

  if(!_inputData.empty() && input(0)->type()==Data::TEX2D && !outputSizeEqualInputSize()) {
    Vector2f texSize(((TextureData *)input(0))->tex()->w()*_tsize[0],
		     ((TextureData *)input(0))->tex()->h()*_tsize[1]);
    size = texSize+_toffset;
  }

  //_offset = size/10.0f;

  for(unsigned int i=0;i<_outputData.size();++i) {
    TextureData *data = (TextureData *)output(i);
    //data->setSize(size+offset());
    data->createTex(size);
  }

  for(unsigned int i=0;i<_tmpData.size();++i) {
    TextureData *data = (TextureData *)tmp(i);
    //data->setSize(size+offset());
    data->createTex(size);
  }

  // create tmp textures and FBO
  initFBO();
  initVertices();
  _ready = true;

  updatePositions();
}

void GenericCustomNode::setIONumbers(unsigned int nbInputs,unsigned int nbOutputs) {
  if(nbInputs==_nbInputs && nbOutputs==_nbOutputs) return;

  _ready     = false;
  _nbInputs  = nbInputs;
  _nbOutputs = nbOutputs;

  initEmptyData(pos());
  const vector<pair<NodeInterface *,Vector2u> > c; 
  setInputData(c);

  if(_nbInputs==0) initOutputData();
}

void GenericCustomNode::setIONames(const QStringList &inames,const QStringList &onames) {
  if(inames.size()!=(int)nbInputs() || onames.size()!=(int)nbOutputs()) {
    qDebug() << "setIONames error: size name!=nbIO" << endl;
    return;
  }

  if(!handle()) {
    qDebug() << "setIONames error: handle is null" << endl;
    return;
  }

  _inames = inames;
  _onames = onames;
}

const QStringList GenericCustomNode::inputDesc() {
  return _inames;
}

const QStringList GenericCustomNode::outputDesc() {
  return _onames;
}
