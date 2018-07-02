// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <iostream>

#include "surfaceFlowsImage.h"
#include "misc/glutils.h"

using namespace std;

SfImageToolWidget::SfImageToolWidget(NodeInterface *node) 
  : NodeWidget(node),
    _alphaShading(new KFSliderFloat(node,"alphaShading",-5,5,0)),
    _betaShading(new KFSliderFloat(node,"betaShading",0,10,0)),
    _gammaTexture(new KFSliderFloat(node,"gammaTexture",0,10,0)),
    _gammaCorrect(new KFSliderFloat(node,"gammaCorrection",0,10,1)),
    _type(new ComboBoxInt(node,"deformation type",QStringList()<<"Shading"<<"Bumps",0)),
    _mode(new ComboBoxInt(node,"texture mode",QStringList()<<"Clamp"<<"Repeat"<<"Mirror",0)),
    _colorPicker(new KFColorPicker(node,"color")),
    _deformBox(new QGroupBox("Deformation")),
    _behaviorBox(new QGroupBox("Behavior")),
    _materialBox(new QGroupBox("Material")),
    _translation(new KFEdit2Float(node,"translation",Vector2f(-5,-5),Vector2f(5,5),Vector2f(0,0))),
    _rotation(new KFSliderFloat(node,"rotation",0,2*M_PI,0)),
    _scaling(new KFSliderFloat(node,"scaling",0.25,6,1)) {

  QVBoxLayout *l;
  l = new QVBoxLayout();
  l->addWidget(_alphaShading);
  l->addWidget(_betaShading);
  l->addWidget(_gammaTexture);
  _deformBox->setLayout(l);

  l = new QVBoxLayout();
  l->addWidget(_type);
  l->addWidget(_mode);
  l->addWidget(_translation);
  l->addWidget(_rotation);
  l->addWidget(_scaling);
  _behaviorBox->setLayout(l);
  
  l = new QVBoxLayout();
  l->addWidget(_colorPicker);
  l->addWidget(_gammaCorrect);
  _materialBox->setLayout(l);
  
  l = new QVBoxLayout();
  l->addWidget(_deformBox);
  l->addWidget(_materialBox);
  l->addWidget(_behaviorBox);
  setLayout(l);

  addChildWidget(_alphaShading);
  addChildWidget(_betaShading);
  addChildWidget(_gammaTexture);
  addChildWidget(_gammaCorrect);
  addChildWidget(_colorPicker);
  addChildWidget(_type);
  addChildWidget(_mode);
  addChildWidget(_translation);
  addChildWidget(_rotation);
  addChildWidget(_scaling);
}

SfImageToolNode::SfImageToolNode(PbGraph *parent,NodeHandle *handle)
  : NodeTexture2D(parent,handle),
    _p(QString(plugDir()+"/textureNodes/surfaceFlowsImage/surfaceFlowsImage.vert").toStdString(),
       QString(plugDir()+"/textureNodes/surfaceFlowsImage/surfaceFlowsImage.frag").toStdString()),
    _w(new SfImageToolWidget(this)) {
  
  _p.addUniform("object");
  _p.addUniform("image");
  _p.addUniform("alphaShading");
  _p.addUniform("betaShading");
  _p.addUniform("gammaTexture");
  _p.addUniform("gammaCorrect");
  _p.addUniform("materialColor");
  _p.addUniform("anchorPoint");
  _p.addUniform("deformType");
  _p.addUniform("imageOrientation");
  _p.addUniform("imageScale");
}

void SfImageToolNode::apply() {
  // save original wrapping mode 
  GLint origWrap;
  inputTex(1)->bind();
  _glf->glGetTexParameteriv(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,&origWrap);

  // set texture wrap 
  GLint wrap = GL_CLAMP_TO_EDGE; // default: clamp
  switch(_w->mode()->val()) {
  case 1: wrap = GL_REPEAT;          break;
  case 2: wrap = GL_MIRRORED_REPEAT; break;
  }
  _glf->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,wrap);
  _glf->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,wrap);

  // set viewport 
  Glutils::setViewport(outputTex(0)->w(),outputTex(0)->h());

  // send parameters 
  _p.enable();
  _p.setUniformTexture("object"          ,GL_TEXTURE_2D,inputTex(0)->id());
  _p.setUniformTexture("image"           ,GL_TEXTURE_2D,inputTex(1)->id());
  _p.setUniform1f     ("alphaShading"    ,_w->alphaShading()->val());
  _p.setUniform1f     ("betaShading"     ,_w->betaShading()->val());
  _p.setUniform1f     ("gammaTexture"    ,_w->gammaTexture()->val());
  _p.setUniform1f     ("gammaCorrect"    ,_w->gammaCorrect()->val());
  _p.setUniform4fv    ("materialColor"   ,(GLfloat *)(_w->colorPicker()->val().data()));
  _p.setUniform2fv    ("anchorPoint"     ,(GLfloat *)(_w->translation()->val().data()));
  _p.setUniform1i     ("deformType"      ,_w->type()->val());
  _p.setUniform1f     ("imageOrientation",_w->rotation()->val());
  _p.setUniform1f     ("imageScale"      ,_w->scaling()->val());

  // draw!
  drawOutputs(buffersOfOutputTex(0),nbOutputs(),true,true);
  _p.disable();

  // back to original state 
  inputTex(1)->bind();
  _glf->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,origWrap);
  _glf->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,origWrap);
}

void SfImageToolNode::reload() {
  _p.reload();
}

void SfImageToolNode::mousePressEvent(const Vector2f &m,QMouseEvent *e) {
  const Vector2f nmouse(m[0]/(float)outputTex(0)->w(),m[1]/(float)outputTex(0)->h());

  if(e->button()==Qt::LeftButton) {
    // translation
    _w->translation()->setVal(nmouse);
    _interact = TRANSLATE;
  } else if(e->button()==Qt::RightButton) {
    // rotation
    const Vector2f v1 = (nmouse-_w->translation()->val()).normalized();
    const Vector2f v2(1,0);
    _tmp = Vector2f(_w->rotation()->val(),v1[1]<0.0f ?  2.0f*M_PI-acos(v1.dot(v2)) : acos(v1.dot(v2)));
    _interact = ROTATE;
  } else if(e->button()==Qt::MidButton) {
    // scaling
    _tmp[0] = nmouse[1];
    _interact = SCALE;
  }
}

void SfImageToolNode::mouseMoveEvent(const Vector2f &m,QMouseEvent *) {
  const Vector2f nmouse(m[0]/(float)outputTex(0)->w(),m[1]/(float)outputTex(0)->h());

  if(_interact==TRANSLATE) {
    // translation
    _w->translation()->setVal(nmouse);
  } else if(_interact==ROTATE) {
    // rotation
    const Vector2f v1 = (nmouse-_w->translation()->val()).normalized();
    const Vector2f v2(1,0);
    const float angle = v1[1]<0.0f ?  2.0f*M_PI-acos(v1.dot(v2)) : acos(v1.dot(v2));
    const float rot = _tmp[0] + (angle-_tmp[1]);
    const float mrot = (float)fmod((double)rot,(double)(2.0f*M_PI));
    _w->rotation()->setVal(mrot<0.0f ? mrot+2.0f*M_PI : mrot);
  } else if(_interact==SCALE) {
    // scaling
    const float d = _tmp[0]-nmouse[1];
    const float s = d<0.0f ? 1.05f : 0.95f;
    _w->scaling()->setVal(std::min(std::max(_w->scaling()->val()*s,
					    _w->scaling()->minVal()),
				   _w->scaling()->maxVal()));
    _tmp[0] = nmouse[1];
  }
}

const QString SfImageToolHandle::name() {
  return "sfImageTool";
}

const QString SfImageToolHandle::path() {
  return "rendering/surfaceFlows/";
}

const QString SfImageToolHandle::desc() {
  return "Surface Flows [Vergne et al.2012]";
}

const QString SfImageToolHandle::help() {
  return tr("The amount of blur is controled by the kernel size.<br>"
	    "Increase this size to increase the blur");
}

const QStringList SfImageToolHandle::inputDesc() {
  return (QStringList() << "normalDepth" << "image");
}

const QStringList SfImageToolHandle::outputDesc() {
  return (QStringList() << "surfaceFlows");
}

NodeInterface *SfImageToolHandle::createInstance(PbGraph *parent,unsigned int,unsigned int) {
  return new SfImageToolNode(parent,this);
}

