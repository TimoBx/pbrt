// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "gridCustom.h"
#include <QString>
#include <QDebug>
#include <QStringList>
#include <iostream>

#include "misc/glutils.h"
#include "misc/mesh.h"
#include "core/pbgraph.h"
#include "gridCustomWidget.h"

using namespace std;

GridCustomNode::GridCustomNode(PbGraph *parent,NodeHandle *handle,unsigned int nbInputs,unsigned int nbOutputs)
  : GenericCustomNode(parent,handle,nbInputs,nbOutputs),
    _camera(new TrackballCamera(Vector2i(512,512),Vector3f(0,0,0),1.0f,0)),
    _vbo(NULL),
    _depth(NULL),
    _p(NULL) {

  _onames = QStringList() << "outBuffer0";

  initShaderSource();

  _p = new GPUProgram(_vertText.toStdString(),_fragText.toStdString(),"",false);
  _w = new GridCustomWidget(this);

  initGrid();

  _p->addUniform("model");
  _p->addUniform("view");
  _p->addUniform("proj");
  _p->addUniform("zmin");
  _p->addUniform("zmax");
}

GridCustomNode::~GridCustomNode() {
  delete _p;
  delete _vbo;
  delete _camera;
  delete _depth;
}

void GridCustomNode::apply() {
  Glutils::setViewport(outputTex(0)->w(),outputTex(0)->h());

  _fbo.bind();
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glDrawBuffers(nbOutputs(),buffersOfOutputTex(0));
  glClearColor(0.0f,0.0f,0.0f,0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  _p->enable();
  
  QStringList l = inputDesc();
  //for(unsigned int i=0;i<nbInputs();++i) {
  for(unsigned int i=0;i<(unsigned int)l.size();++i) {
    _p->setUniformTexture(l[i].toStdString(),GL_TEXTURE_2D,inputTex(i)->id());
  }

  _w->sendUniformsToProg(_p);

  _vbo->bind();
  _vbo->enableDrawAndDisable();
  _vbo->unbind();
  _p->disable();

  FramebufferObject::unbind();
  glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);
}

void GridCustomNode::initGrid() {

  // mesh creation 
  vector<Vector3f> vertices;
  vector<Vector2f> uvs;
  vector<Vector3i> indices;

  const unsigned int samples = (1 << _w->gridDepth()->val());

  const float size = 1.0;
  const float step = size/((float)samples-1);
  const Vector3f start = Vector3f(-0.5f,-0.5f,0.0f)*size;

  for(unsigned int i=0;i<samples;++i) {
    for(unsigned int j=0;j<samples;++j) {
      const Vector3f current = start + Vector3f((float)j,(float)i,0.0f)*step;
      vertices.push_back(current);
      uvs.push_back(Vector2f((float)j,(float)i)/(float)samples);

      if(i>0 && j>0) {
	unsigned int i1 = i*samples+j;
	unsigned int i2 = (i-1)*samples+j;
	unsigned int i3 = (i-1)*samples+j-1;
	unsigned int i4 = i*samples+j-1;
	
	indices.push_back(Vector3i(i1,i2,i3));
	indices.push_back(Vector3i(i1,i3,i4));
      }
    }
  }

  delete _vbo;
  _vbo = new VertexbufferObject();
  unsigned int nbVert = vertices.size();
  unsigned int nbInd  = indices.size();
  
  _vbo->setAttrib(nbVert*sizeof(Vector3f),&(vertices[0][0]),0,3);
  _vbo->setAttrib(nbVert*sizeof(Vector2f),&(uvs[0][0]),1,2);
  _vbo->setIndices(nbInd*sizeof(Vector3i),&(indices[0][0]),nbInd*3);
}

void GridCustomNode::initCam() {
  Vector2i size(outputSize()[0],outputSize()[1]);
  _camera->reinit(size,_camera->sceneCenter(),_camera->sceneRadius());
}

void GridCustomNode::initProg() {
  delete _p; _p = NULL;
  _p = new GPUProgram(_vertText.toStdString(),_fragText.toStdString(),"",false);
  ((GridCustomWidget *)widget())->setLogFromProg(_p);
  ((GridCustomWidget *)widget())->addUniformsToProg(_p);
}

void GridCustomNode::mousePressEvent(const Vector2f &p,QMouseEvent *me) {
  if(me->button()==Qt::LeftButton) {
    _camera->initRotation(p);
  } else if(me->button()==Qt::RightButton) {
    _camera->initMoveXY(p);
  } else if(me->button()==Qt::MidButton) {
    _camera->initMoveZ(p);
  }
  update();
}

void GridCustomNode::mouseMoveEvent(const Vector2f &p,QMouseEvent *) {
  _camera->move(p);
  update();
}

void GridCustomNode::wheelEvent(const Vector2f &p,QWheelEvent *we) {
  const Vector2f v(0.0f,60.0f);

  _camera->initMoveZ(p);
  if(we->delta()>0) {
    _camera->move(p+v);
  } else {
    _camera->move(p-v);
  }
  update();
}


bool GridCustomNode::save(SceneSaver  *saver) {
  bool ret = true;

  // camera 
  if(!saver->saveIOData("camera",_camera))     ret = false;

  if(!GenericCustomNode::save(saver)) ret = false;

  // head and body 
  if(!saver->saveString("genericVertHead",_vertHeadText)) ret = false;
  if(!saver->saveString("genericVertBody",_vertBodyText)) ret = false;
  if(!saver->saveString("genericFragHead",_fragHeadText)) ret = false;
  if(!saver->saveString("genericFragBody",_fragBodyText)) ret = false;

  return ret;
}

bool GridCustomNode::load(SceneLoader *loader) {

  // the number of inputs/outputs should have already been set in thanks to the constructor
  bool ret = true;

  // load cam
  if(!loader->loadIOData("camera",_camera))       ret = false;
  initGrid();

  if(!GenericCustomNode::load(loader)) ret = false;

  // head and body 
  if(!loader->loadString("genericVertHead",_vertHeadText)) ret = false;
  if(!loader->loadString("genericVertBody",_vertBodyText)) ret = false;
  if(!loader->loadString("genericFragHead",_fragHeadText)) ret = false;
  if(!loader->loadString("genericFragBody",_fragBodyText)) ret = false;

  setBodies(_vertBodyText,_fragBodyText);
  _w->setVertSource(_vertHeadText,_vertBodyText);
  _w->setFragSource(_fragHeadText,_fragBodyText);

  return ret;
}

void GridCustomNode::reload() {
  if(_p) _p->reload();
}

void GridCustomNode::initFBO() {  
  NodeTexture2D::initFBO();

  _depth = new FloatTexture2D(TextureFormat(GL_TEXTURE_2D,outputSize()[0],outputSize()[1],
  					    GL_DEPTH_COMPONENT24,GL_DEPTH_COMPONENT,GL_FLOAT),
  			      TextureParams(GL_NEAREST,GL_NEAREST));

  _fbo.bind();
  _fbo.attachTexture(GL_TEXTURE_2D,_depth->id(),GL_DEPTH_ATTACHMENT);
  _fbo.isValid();

  FramebufferObject::unbind();
}

void GridCustomNode::cleanFBO() {
  NodeTexture2D::cleanFBO();
  delete _depth; _depth = NULL;
}


void GridCustomNode::initShaderSource() {

  _vertHeadText = QObject::tr("#version 330\n\n"
			      "layout(location = 0) in vec3 inVertex;\n"
			      "layout(location = 1) in vec2 inTexcoord;\n\n"
			      "uniform mat4 model;\n"
			      "uniform mat4 view;\n"
			      "uniform mat4 proj;\n"
			      "uniform float zmin;\n"
			      "uniform float zmax;\n");

  _vertBodyText = QObject::tr("out vec3  normalV;\n"
			      "out float depthV;\n\n"
			      "void main() {\n"
			      "\tmat4 mdv    = view*model;\n"
			      "\tmat4 mvp    = proj*mdv;\n\n"
			      "\tnormalV     = (mdv*vec4(0,0,1,0)).xyz;\n"
			      "\tdepthV      = (clamp(-(mdv*vec4(inVertex,1)).z,zmin,zmax)-zmin)/(zmax-zmin);\n"
			      "\tgl_Position = mvp*vec4(inVertex,1);\n"
			      "}\n");

  _fragHeadText = QObject::tr("#version 330\n\n"
			      "uniform mat4 model;\n"
			      "uniform mat4 view;\n"
			      "uniform mat4 proj;\n"
			      "uniform float zmin;\n"
			      "uniform float zmax;\n\n"
			      "layout(location = 0) out vec4 outBuffer0;\n");

  _fragBodyText = QObject::tr("in vec3  normalV;\n"
			      "in float depthV;\n\n"
			      "void main() {\n"
			      "\toutBuffer0 = vec4(normalize(normalV),depthV);\n"
			      "}\n");

  _vertText = _vertHeadText+_vertBodyText;
  _fragText = _fragHeadText+_fragBodyText;
}

void GridCustomNode::setHeads(const QString &vertHead,const QString &fragHead) {
  _vertHeadText = vertHead;
  _fragHeadText = fragHead;
  _vertText = _vertHeadText+_vertBodyText;
  _fragText = _fragHeadText+_fragBodyText;
  initProg();
}

void GridCustomNode::setBodies(const QString &vertBody,const QString &fragBody) {
  _vertBodyText = vertBody;
  _fragBodyText = fragBody;
  _vertText = _vertHeadText+_vertBodyText;
  _fragText = _fragHeadText+_fragBodyText;
  initProg();
}

void GridCustomNode::setImgSize(const Vector2f &tsize,const Vector2f &toffset) {
  GenericCustomNode::setImgSize(tsize,toffset);

  const Vector2f o(outputSize());
  _camera->setViewport(Vector2i((int)o[0],(int)o[1]));
}

GridCustomHandle::GridCustomHandle() {
}

const QString GridCustomHandle::name() {
  return "gridBuffGeneric";
}

const QString GridCustomHandle::path() {
  return "generic/";
}

const QString GridCustomHandle::desc() {
  return "Generic node for 3D grid processing and gbuffers creation";
}

const QString GridCustomHandle::help() {
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

const QStringList GridCustomHandle::inputDesc()  {return QStringList();}

const QStringList GridCustomHandle::outputDesc() {return QStringList();}

NodeInterface *GridCustomHandle::createInstance(PbGraph *parent,unsigned int nbInputs,unsigned int nbOutputs) {
  return (nbInputs==0 && nbOutputs==0) ? new GridCustomNode(parent,this,0,1) : new GridCustomNode(parent,this,nbInputs,nbOutputs);
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(gridCustom,GridCustomHandle)
QT_END_NAMESPACE

