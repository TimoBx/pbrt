// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "imgCustom.h"
#include <QString>
#include <QDebug>
#include <QStringList>
#include <iostream>

#include "misc/glutils.h"
#include "core/pbgraph.h"
#include "imgCustomWidget.h"

using namespace std;

ImgCustomNode::ImgCustomNode(PbGraph *parent,NodeHandle *handle,unsigned int nbInputs,unsigned int nbOutputs)
  : GenericCustomNode(parent,handle,nbInputs,nbOutputs),
    _p(NULL) {

  _inames = QStringList() << "inBuffer0";
  _onames = QStringList() << "outBuffer0";

  initShaderSource();

  _p = new GPUProgram(_vertText.toStdString(),_fragText.toStdString(),"",false);
  _w = new ImgCustomWidget(this);
  _p->addUniform("inBuffer0");
}

ImgCustomNode::~ImgCustomNode() {
  delete _p;
}

void ImgCustomNode::apply() {
  Glutils::setViewport(outputTex(0)->w(),outputTex(0)->h());
  
  _p->enable();

  QStringList l = inputDesc();
  //for(unsigned int i=0;i<nbInputs();++i) {
  for(unsigned int i=0;i<(unsigned int)l.size();++i) {
    _p->setUniformTexture(l[i].toStdString(),GL_TEXTURE_2D,inputTex(i)->id());
  }

  _w->sendUniformsToProg(_p);
  drawOutputs(buffersOfOutputTex(0),nbOutputs(),true,true);
  _p->disable();
}

void ImgCustomNode::initProg() {
  delete _p; _p = NULL;
  _p = new GPUProgram(_vertText.toStdString(),_fragText.toStdString(),"",false);
  _w->setLogFromProg(_p);
  _w->addUniformsToProg(_p);
}

bool ImgCustomNode::save(SceneSaver  *saver) {
  bool ret = GenericCustomNode::save(saver);

  // head and body 
  if(!saver->saveString("genericHead",_headText)) ret = false;
  if(!saver->saveString("genericBody",_bodyText)) ret = false;

  return ret;
}

bool ImgCustomNode::load(SceneLoader *loader) {

  // the number of inputs/outputs should have already been set in thanks to the constructor
  bool ret = GenericCustomNode::load(loader);

  // head and body 
  if(!loader->loadString("genericHead",_headText)) ret = false;
  if(!loader->loadString("genericBody",_bodyText)) ret = false;

  setBody(_bodyText);
  _w->setSource(_headText,_bodyText);
  
  return ret;
}

void ImgCustomNode::reload() {
  if(_p) _p->reload();
}


void ImgCustomNode::initShaderSource() {
  _vertText = QObject::tr("#version 330\n\n"
			  "layout(location = 0) in vec2 vertex;\n"
			  "layout(location = 1) in vec2 coord;\n\n"
			  "out vec2 texcoord;\n\n"
			  "void main() {\n"   
			  "\tgl_Position  = vec4(vertex,0,1);\n"
			  "\ttexcoord     = coord;\n"
			  "}\n");

  _headText = QObject::tr("#version 330\n\n"
			  "in vec2 texcoord;\n\n"
			  "layout(location = 0) out vec4 outBuffer0;\n"
			  "uniform sampler2D inBuffer0;\n");

  _bodyText = QObject::tr("void main() {\n"
			  "\toutBuffer0 = texture(inBuffer0,texcoord);\n"
			  "}\n");

  _fragText = _headText+_bodyText;
}

void ImgCustomNode::setHead(const QString &head) {
  _headText = head;
  _fragText = _headText+_bodyText;

  initProg();
}

void ImgCustomNode::setBody(const QString &body) {
  _bodyText = body;
  _fragText = _headText+_bodyText;

  initProg();
}



ImgCustomHandle::ImgCustomHandle() {
}

const QString ImgCustomHandle::name() {
  return "imgGeneric";
}

const QString ImgCustomHandle::path() {
  return "generic/";
}

const QString ImgCustomHandle::desc() {
  return "Generic node for image processing";
}

const QString ImgCustomHandle::help() {
  return QObject::tr("This node was designed to create custom shaders\n"
		     "The settings widget allows to modify input, output parameters\n"
		     "as well as the output image sizes.\n\n"
		     "WARNING1: you will be able to modify the number of inputs/outputs\n"
		     "only if the node is entirely disconnected.\n"
		     "WARNING2: input/output names will not be modified if a custom node is created\n"
		     "from this node beacause these names should be edited directly in the setting widget\n"
		     "WARNING3: modifying the settings of a node will remove all previously\n"
		     "defined keyframes\n\n" 
		     "* Special data allows to modify the output image (using a multiple of\n"
		     "the input size, plus an offset vector\n"
		     "Check use mouse if you want to interact with the mouse\n\n"
		     "* Input data allows to add/remove/displace input data (only if disconnected)\n\n"
		     "* Output data allows to add/remove/displace output data (only if disconnected)\n\n"
		     "* Param data allows to add/remove parameters that will automatically be \n"
		     "included in your shader.\n"
		     "For each of them you may choose a proper name, type, and min/max/default values.\n"
		     "Check keyframes if you want your parameter to be accessible\n"
		     " through the animation widget\n\n"
		     "In the node interface itself:\n"
		     "Source tab contains the head and body of your GLSL source (only the body is editable)\n"
		     "Params tab contains the defined widgets according to your setting choices\n"
		     "Log tab contains compilation errors\n\n"
		     "Once your settings and source completed, click on apply to see the result\n"
		     );
}

const QStringList ImgCustomHandle::inputDesc() {
  return QStringList();
}

const QStringList ImgCustomHandle::outputDesc() {
  return QStringList();
}

NodeInterface *ImgCustomHandle::createInstance(PbGraph *parent,unsigned int nbInputs,unsigned int nbOutputs) {
  return (nbInputs==0 && nbOutputs==0) ? new ImgCustomNode(parent,this,1,1) : new ImgCustomNode(parent,this,nbInputs,nbOutputs);
}

