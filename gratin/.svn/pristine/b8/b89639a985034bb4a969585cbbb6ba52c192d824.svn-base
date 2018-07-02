// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "view/graphWidget.h"
#include "view/customNodeWidget.h"
#include "core/nodeInterface.h"
#include "core/nodeHandle.h"
#include "core/gremlin.h"
#include "misc/glutils.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <QDebug>
#include <QMessageBox>

using namespace std;

GraphWidget::GraphWidget(const QGLFormat &format)
  : QGLWidget(format),
    _nodeInterfaceWidget(NULL),
    _nodeListWidget(NULL),
    _interactionWidget(NULL),
    _animationWidget(NULL),
    _nodeManager(NodeManager::instance()),
    _pbGraph(new PbGraph()),
    _animation(new Animation()),
    _scale(2.0f),
    _interaction(NO_MOTION),
    _clickState(NOTHING_HAS_BEEN_CLICKED),
    _mouseCoord(0,0),
    _mouseTmp(0,0),
    _offset(0.0f,0.0f),
    _ctrl(false),
    _shift(false),
    _outNodeSelection(-1,-1),
    _inNodeSelection(-1,-1),
    _pgraph(NULL),
    _copier(NULL),
    _paster(NULL) {

  setFocusPolicy(Qt::StrongFocus);
  setFocus();
  setlocale(LC_ALL,"C");

  srand(time(NULL));

  connect(_pbGraph,SIGNAL(nodeContentChanged()),this,SLOT(updateGL()));
}

GraphWidget::~GraphWidget() {
  delete _pbGraph;
  delete _nodeManager;
  delete _pgraph;
  delete _animation;
  delete _copier;
  delete _paster;
}

void GraphWidget::paintGL() {
   glClearColor(0.86f,0.86f,0.90f,1.0f);
  //glClearColor(1.0f,1.0f,1.0f,1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glEnable(GL_BLEND); 
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

  const Matrix4f mvp = Glutils::screenMode(width(),height(),_offset,_scale);

  // draw graph connections
  drawGraphConnections(mvp);

  // draw graph nodes 
  for(unsigned int i=0;i<_pbGraph->nbNodes();++i) {
    _pbGraph->node(i)->draw(mvp);
  }

  // draw motion connection
  switch(_interaction) {
  case CONNECT_MOTION: 
    // the user is currently moving the output connection of a node 
    drawConnection(mvp);
    break;
  case MULTIPLE_SELECT_MOTION: 
    // the user is currently selecting a group of elements
    drawSelection(mvp);
    break;
  default:
    break;
  }

  glDisable(GL_BLEND); 

  // update interaction widget
  _interactionWidget->updateGL();
}


void GraphWidget::mousePressEvent(QMouseEvent *me) {
  _mouseCoord = remapCoord(me->x(),height()-me->y());

  if(me->button()==Qt::MidButton) {
    // zoom in and out
    _interaction = SCALING_MOTION;
    _mouseTmp    = Vector2f(me->x(),height()-me->y());
  } else if(me->button()==Qt::RightButton) {
    // translation
    _interaction = OFFSET_MOTION;
    _mouseTmp    = Vector2f(me->x(),height()-me->y());
  } else if(me->button()==Qt::LeftButton) {

    // first find what is clicked
    if(inputNodeClicked(_mouseCoord) && inputConnected(_inNodeSelection)) {
      NodeInterface *src = _pbGraph->node(_outNodeSelection[0]);
      NodeInterface *dst = _pbGraph->node(_inNodeSelection[0]);
      if(_pbGraph->removeConnection(src,_outNodeSelection[1],dst,_inNodeSelection[1])) {
	_outNodeSelection[0] = src->nodeId();
	_inNodeSelection[0] = dst->nodeId();
	graphStateChanged();
	LogWidget::instance()->setText("Connection between "+src->nameId()+" and "+dst->nameId()+" removed from the graph");
      }
      _clickState = AN_OUTPUT_HAS_BEEN_CLICKED;
      _interaction = CONNECT_MOTION;
      _mouseTmp = remapCoord(me->x(),height()-me->y());
    } else if(outputNodeClicked(_mouseCoord)) {
      _clickState = AN_OUTPUT_HAS_BEEN_CLICKED;
     _interaction = NO_MOTION;
    } else if(nodeClicked(_mouseCoord)) {
      _mouseTmp   = Vector2f(me->x(),height()-me->y());
      _clickState = A_NODE_HAS_BEEN_CLICKED;
     _interaction = NO_MOTION;
    } else {
      _mouseTmp   = Vector2f(me->x(),height()-me->y());
      _clickState = NOTHING_HAS_BEEN_CLICKED;
     _interaction = NO_MOTION;
    }
  }

  updateGL();
}

void GraphWidget::mouseMoveEvent(QMouseEvent *me) {

  if(_interaction==SCALING_MOTION) {
    // zoom in/out
    if(((height()-me->y())-_mouseTmp[1])>0 && _scale<100.0f) {
      _scale*=1.05f;
    } else if(((height()-me->y())-_mouseTmp[1])<0 && _scale>0.02f) {
      _scale*=0.95f;
    }
    _mouseTmp = Vector2f(me->x(),height()-me->y());

  } else if(_interaction==OFFSET_MOTION) {
    // background translations
    _offset  -= Vector2f(me->x()-_mouseTmp[0],(height()-me->y())-_mouseTmp[1])*_scale;
    _mouseTmp = Vector2f(me->x(),height()-me->y());
  } else if(_interaction==NO_MOTION && _clickState==AN_OUTPUT_HAS_BEEN_CLICKED) {
    // the user will make a new connection
    _interaction = CONNECT_MOTION;
  } else if(_interaction==NO_MOTION && _clickState==A_NODE_HAS_BEEN_CLICKED) {
    
    if(clickedNodeSelected()) {
      // the node is already selected, we move everything
      _interaction = SELECT_MOTION;
    } else {
      // the node is not selected. Unselect everything and select this node
      unselectAll();
      selectClickedNode();
      _interaction = SELECT_MOTION;
    }

  } else if(_interaction==NO_MOTION && _clickState==NOTHING_HAS_BEEN_CLICKED) {
    // create a rectangle selection
    _interaction = MULTIPLE_SELECT_MOTION;
  }


  if(_interaction==CONNECT_MOTION) {
    // the output position has been clicked and the user makes a new connection
    _mouseTmp = remapCoord(me->x(),height()-me->y());
  } else if(_interaction==SELECT_MOTION) {
    // the user clicked on a node and displace it (and possibly all the other selected nodes)
    moveSelection(Vector2f(me->x()-_mouseTmp[0],(height()-me->y())-_mouseTmp[1])*_scale);
    _mouseTmp = Vector2f(me->x(),height()-me->y());
  } else if(_interaction==MULTIPLE_SELECT_MOTION) {
    _mouseTmp = remapCoord(me->x(),height()-me->y());
  }

  updateGL();
}

void GraphWidget::mouseReleaseEvent(QMouseEvent *me) {
  if(_interaction==CONNECT_MOTION) {
    // check if the final coord corresponds to an input and connect
    _mouseTmp = remapCoord(me->x(),height()-me->y());

    if(inputNodeClicked(_mouseTmp)) {
      NodeInterface *src = _pbGraph->node(_outNodeSelection[0]);
      NodeInterface *dst = _pbGraph->node(_inNodeSelection[0]);
      if(_pbGraph->makeConnection(src,_outNodeSelection[1],dst,_inNodeSelection[1])) {
	graphStateChanged();
	_pbGraph->applyFrom(dst);
	LogWidget::instance()->setText("Connection between "+src->nameId()+" and "+dst->nameId()+" added to the graph");
      } else
        LogWidget::instance()->setText("Connection not allowed!");
    } 

  } else if(_interaction==MULTIPLE_SELECT_MOTION) {
    unselectNodeOutputs();

    // select / unselect the rectangle selection
    if(_ctrl) selectSelection();
    else if(_shift) unselectSelection();
    else {unselectAll(); selectSelection();}
  } else if(_interaction==NO_MOTION && _clickState==A_NODE_HAS_BEEN_CLICKED) {
    unselectNodeOutputs();

    // select / unselect the selection
    if(_ctrl) selectClickedNode();
    else if(_shift) unselectClickedNode();
    else {
      unselectAll(); 
      selectClickedNode();
      //TODO: test if an output has been clicked here
      selectNodeOutput();
    }
  } else if(_interaction==NO_MOTION && _clickState==NOTHING_HAS_BEEN_CLICKED) {
    // single click on background: unselected everything
    unselectAll();
  } 

  // back to initial state
  _interaction = NO_MOTION;
  _clickState  = NOTHING_HAS_BEEN_CLICKED;

  updateGL();
  setFocus();
}


void GraphWidget::mouseDoubleClickEvent(QMouseEvent *me) {
  if(_interaction==NO_MOTION && me->button()==Qt::LeftButton && _pbGraph->nbNodes()>(unsigned int)_outNodeSelection[0]) {
    // highlight the corresponding widget 
    NodeInterface *n = _pbGraph->node(_outNodeSelection[0]);
    NodeWidget *w = n->widget();
    _nodeInterfaceWidget->highlightWidget(w);
    if(w) {
      _nodeInterfaceWidget->show();
      LogWidget::instance()->setText("Widget of node "+n->nameId());
    } else
      LogWidget::instance()->setText("Node "+n->nameId()+" -- no widget");

    mousePressEvent(me);  
  } // else if(me->button()==Qt::RightButton) {

  // }
}

void GraphWidget::wheelEvent(QWheelEvent *we) {
  if(we->delta()>0 && _scale<100.0f) {
    _scale*=1.1f;
  } else if(we->delta()<0 && _scale>0.02f) {
    _scale*=0.9f;
  }
  updateGL();
}


void GraphWidget::keyPressEvent(QKeyEvent *ke) {
  if(ke->modifiers() & Qt::ControlModifier) {
    // control is pressed
    _ctrl = true;
  } 

  if(ke->modifiers() & Qt::ShiftModifier) {
    // shift is pressed
    _shift = true;
  } 

  if(ke->key()==Qt::Key_A && _ctrl) {
    // select/unselect everything
    if(everythingIsSelected()) {
      unselectAll();
    } else {
      selectAll();
    }
  } else if(ke->key()==Qt::Key_Escape) {
    unselectAll();
  } else if(ke->key()==Qt::Key_Delete) {
    removeSelection();
  } else if(ke->key()==Qt::Key_Space) {
    if(_outNodeSelection[0]<0 || _outNodeSelection[1]<0) return;
    
    NodeInterface *node = _pbGraph->node(_outNodeSelection[0]);
    unsigned int outId = _outNodeSelection[1];
    if(_interactionWidget->containNode(node,outId)) {
      _interactionWidget->removeNode(node,outId);
      LogWidget::instance()->setText("Removing output "+QString::number(outId)+" of node "+node->nameId()+" from the interaction widget");
    } else {
      _interactionWidget->addNode(node,outId);
      LogWidget::instance()->setText("Adding output "+QString::number(outId)+" of node "+node->nameId()+" to the interaction widget");
    }
  }

  updateGL();
}

void GraphWidget::keyReleaseEvent(QKeyEvent *ke) {
  if(!(ke->modifiers() & Qt::ControlModifier)) {
    _ctrl = false;
  } 

  if(!(ke->modifiers() & Qt::ShiftModifier)) {
    _shift = false;
  } 

  updateGL();
}

void GraphWidget::zoom(int type) {
  switch(type) {
  case 0: _scale = 1.0f; break;
  case 1: _scale = _scale*0.5f;break;
  case -1: _scale = _scale*2.0f;break;
  }
  updateGL();
}

void GraphWidget::drawConnection(const Matrix4f &mvp) {
  const float size = 15.0f;
  const Vector3f pColor(0.7f,0.3f,0.3f);
  const Vector3f lColor(0.3f,0.3f,0.8f);

  vector<Vector2f> pts(2);

  pts[0] = _pbGraph->node(_outNodeSelection[0])->outputNodeCoord(_outNodeSelection[1]);
  pts[1] = _mouseTmp;

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,pts[0].data());
  _pgraph->enable();
  _pgraph->setUniformMatrix4fv("mvp",(GLfloat *)mvp.data());
  _pgraph->setUniform1f("size",size);
  _pgraph->setUniform3fv("color",(GLfloat *)lColor.data());
  glDrawArrays(GL_LINES,0,2);
  _pgraph->setUniform3fv("color",(GLfloat *)pColor.data());
  glDrawArrays(GL_POINTS,1,1);
  _pgraph->disable();
  glDisableVertexAttribArray(0);
}

void GraphWidget::drawGraphConnections(const Matrix4f &mvp) {
  const Vector3f lColor(0.3f,0.3f,0.8f);
  const PbGraph::AdjacencyMatrix m = _pbGraph->adjacencyMatrix();

  vector<Vector2f> pts;
  
  for(unsigned int i=0;i<m.size();++i) {
    for(unsigned int j=i+1;j<m[i].size();++j) {
      for(unsigned int k=0;k<m[i][j].size();++k) {
	const Vector2u oi = m[i][j][k];
	NodeInterface *src = _pbGraph->node(i);
	NodeInterface *dst = _pbGraph->node(j);

	pts.push_back(src->outputNodeCoord(oi[0]));
	pts.push_back(dst->inputNodeCoord(oi[1]));
      }
    }
  }

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,pts[0].data());
  _pgraph->enable();
  _pgraph->setUniformMatrix4fv("mvp",(GLfloat *)mvp.data());
  _pgraph->setUniform3fv("color",(GLfloat *)lColor.data());
  glDrawArrays(GL_LINES,0,pts.size());
  _pgraph->disable();
  glDisableVertexAttribArray(0);
}

void GraphWidget::drawSelection(const Matrix4f &mvp) {
  const Vector3f color(0.7f,0.3f,0.3f);

  vector<Vector2f> pts(4);
  pts[0] = _mouseCoord;
  pts[1] = Vector2f(_mouseTmp[0],_mouseCoord[1]);
  pts[2] = _mouseTmp;
  pts[3] = Vector2f(_mouseCoord[0],_mouseTmp[1]);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,pts[0].data());
  _pgraph->enable();
  _pgraph->setUniformMatrix4fv("mvp",(GLfloat *)mvp.data());
  _pgraph->setUniform3fv("color",(GLfloat *)color.data());
  glDrawArrays(GL_LINE_LOOP,0,4);
  _pgraph->disable();
  glDisableVertexAttribArray(0);
}


void GraphWidget::initializeGL() {
  makeCurrent();

  if(glewInit()!=GLEW_OK) {
    cerr << "Warning: glewInit failed!" << endl;
  }

  if(!GLEW_ARB_vertex_program   ||
     !GLEW_ARB_fragment_program ||
     !GLEW_ARB_texture_float    ||
     !GLEW_ARB_draw_buffers     ||
     !GLEW_ARB_framebuffer_object) {
    cerr << "Warning: Shaders not supported!" << endl;
  }

  glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
  glDepthMask(GL_FALSE);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_PROGRAM_POINT_SIZE);
  glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
  glLineWidth(2.0f);
  glClearColor(0.78f,0.84f,0.87f,1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  _pgraph = new GPUProgram(":/node-connect.vert",":/node-connect.frag");
  _pgraph->addUniform("mvp");
  _pgraph->addUniform("size");
  _pgraph->addUniform("color");

  qDebug() << "OpenGL Version: " << format().majorVersion() << "." << format().minorVersion() << endl;
}

void GraphWidget::resizeGL(int width,int height) {
  glViewport(0,0,width,height);
  glClear(GL_COLOR_BUFFER_BIT);
  QGLWidget::resizeGL(width,height);
  updateGL();
}

void GraphWidget::setNodeListWidget(NodeListWidget *nodeListWidget) {
  _nodeListWidget = nodeListWidget;
  if(!_nodeListWidget) return;

  _nodeListWidget->setNodeList(_nodeManager->nodeHandles());
}

void GraphWidget::setNodeInterfaceWidget(NodeInterfaceWidget *nodeInterfaceWidget) {
  _nodeInterfaceWidget = nodeInterfaceWidget;
}

void GraphWidget::setInteractionWidget(GlInteractionWidget *interactionWidget) {
  _interactionWidget = interactionWidget;
}

void GraphWidget::setAnimationWidget(AnimationWidget *animationWidget) {
  _animationWidget = animationWidget;
}

void GraphWidget::reloadShaders() {
  _pbGraph->reloadAll();
  _pbGraph->applyAll();
  updateGL();
  LogWidget::instance()->setText("Graph reloaded");
}

void GraphWidget::applyNodes() {
  _pbGraph->applyAll();
  updateGL();
}

void GraphWidget::addNode(const QString &name) {
  NodeInterface *node = _nodeManager->createInstance(name,_pbGraph);

  if(!node) return;
  node->initEmptyData(_offset);
  
  if(!(_pbGraph->addNode(node))) {
    delete node;
    return;
  }

  if(_nodeManager->isCustomNode(node->name())) {
    _nodeManager->loadCustomNodeData(node);
  }

  LogWidget::instance()->setText(name+" added to the graph");

  _pbGraph->applyFrom(node);
  graphStateChanged();
  updateGL();
}

void GraphWidget::removeNode(NodeInterface *node) {
  removeFromInteractionWidgets(node);
  _pbGraph->removeNode(node);
  LogWidget::instance()->setText(node->name()+" removed from the graph");
  node->cleanEverything();
  delete node;
  graphStateChanged();
  updateGL();
}

void GraphWidget::graphStateChanged() {
  // change node name ids in the associated widgets 
  _nodeInterfaceWidget->updateIds();
  _animationWidget->updateIds();
}

void GraphWidget::clear() {
  selectAll();
  removeSelection();
}

void GraphWidget::selectUnselectAll() {
  if(everythingIsSelected()) 
    unselectAll();
  else 
    selectAll();
  updateGL();
}

void GraphWidget::copy() {
  delete _copier; _copier = NULL;
  bool ret = true;

  // get selected nodes
  Vector2f meanPos(0,0); 
  vector<unsigned int> selectedNodes;
  for(unsigned int i=0;i<_pbGraph->nbNodes();++i) {
    if(nodeSelected(i)) {
      selectedNodes.push_back(i);
    }
  }

  // copy connections between these nodes 
  const vector<vector<vector<Vector2u> > > adjacency = _pbGraph->adjacencyMatrix();
  vector<vector<vector<Vector2u> > > selectedConnections;
  if(selectedNodes.size()>0)
    selectedConnections = vector<vector<vector<Vector2u> > >(selectedNodes.size());

  for(unsigned int i=0;i<selectedNodes.size();++i) {
    meanPos = meanPos+_pbGraph->node(selectedNodes[i])->pos()/(float)selectedNodes.size();
    selectedConnections[i] = vector<vector<Vector2u> >(selectedNodes.size());
    for(unsigned int j=i+1;j<selectedNodes.size();++j) {
      const unsigned int index1 = selectedNodes[i];
      const unsigned int index2 = selectedNodes[j];
      selectedConnections[i][j] = adjacency[index1][index2];
    }
  }

  // init copier 
  _copier = new SceneSaver();
  _copier->init("selection-copy");
  
  // copy nodes information
  _copier->saveUint("nbNodes",selectedNodes.size());
  _copier->saveVector2f("meanPos",meanPos);

  for(unsigned int i=0;i<selectedNodes.size();++i) {
    NodeManager::instance()->saveNode(_copier,_pbGraph->node(selectedNodes[i]),_pbGraph,ret,i);
  }
  
  // copy node connections 
  for(unsigned int i=0;i<selectedNodes.size();++i) {
    for(unsigned int j=0;j<selectedNodes.size();++j) {
      _copier->saveUint(Tags::attribId("nbConnections",i,j),selectedConnections[i][j].size());   
      for(unsigned int k=0;k<selectedConnections[i][j].size();++k) {
	_copier->saveVector2u(Tags::attribId("connection",i,j,k),selectedConnections[i][j][k]);
      }
    }
  }

  // copy user-defined nodes parameters 
  for(unsigned int i=0;i<selectedNodes.size();++i) {
    NodeManager::instance()->saveNodeData(_copier,_pbGraph->node(selectedNodes[i]),ret,i);
  }

  LogWidget::instance()->setText("Selection copied");
}

void GraphWidget::paste() {
  if(!_copier) return;
  bool ret = true;

  delete _paster; _paster = NULL;
  _paster = new SceneLoader();
  _paster->readFrom(_copier);
  _paster->init("selection-copy");
  
  unsigned int nb = 0;
  Vector2f meanPos(0,0);

  // get nb nodes and create necessary arrays
  _paster->loadUint("nbNodes",nb);
  _paster->loadVector2f("meanPos",meanPos);

  vector<NodeInterface *> selectedNodes;
  vector<vector<vector<Vector2u> > > selectedConnections;
  if(nb>0) {
    selectedNodes = vector<NodeInterface *>(nb,NULL);
    selectedConnections = vector<vector<vector<Vector2u> > >(nb);
    for(unsigned int i=0;i<selectedNodes.size();++i) {
      selectedConnections[i] = vector<vector<Vector2u> >(nb); 
    }
  }

  // create nodes 
  for(unsigned int i=0;i<nb;++i) {
    selectedNodes[i] = NodeManager::instance()->loadNode(_paster,_pbGraph,ret,i);
    if(!selectedNodes[i]) {
      // an error occurred... cancel everything
      for(int j=(int)i-1;j>=0;--j) {
	delete selectedNodes[j];
	LogWidget::instance()->setText("Unable to paste selection...");
	return;
      }
    }
    selectedNodes[i]->initEmptyData(selectedNodes[i]->pos()-meanPos+_offset);
  }

  // get connections 
  for(unsigned int i=0;i<nb;++i) {
    for(unsigned int j=0;j<nb;++j) {
      unsigned int nbConnections=0;
      _paster->loadUint(Tags::attribId("nbConnections",i,j),nbConnections);
      if(nbConnections>0) selectedConnections[i][j]=vector<Vector2u>(nbConnections);
   
      for(unsigned int k=0;k<selectedConnections[i][j].size();++k) {
	_paster->loadVector2u(Tags::attribId("connection",i,j,k),selectedConnections[i][j][k]);
      }
    }
  }

  // insert nodes in the current graph 
  for(unsigned int i=0;i<nb;++i) {
    cout << "adding " << selectedNodes[i]->nameId().toStdString() << " to the graph" << endl;
    _pbGraph->addNode(selectedNodes[i]);
  }

  // insert connections
  for(unsigned int i=0;i<nb;++i) {
    for(unsigned int j=i+1;j<nb;++j) {
      for(unsigned int k=0;k<selectedConnections[i][j].size();++k) {
	cout << "making connection" << endl;
	NodeInterface *src = selectedNodes[i];
	NodeInterface *dst = selectedNodes[j];
	unsigned int output = selectedConnections[i][j][k][0];
	unsigned int input  = selectedConnections[i][j][k][1];
	_pbGraph->makeConnection(src,output,dst,input);
      }
    }
  }

  // load user-defined data 
  for(unsigned int i=0;i<nb;++i) {
    NodeManager::instance()->loadNodeData(_paster,selectedNodes[i],ret,i);
  }

  // apply all new nodes 
  for(unsigned int i=0;i<nb;++i) {
    if(selectedNodes[i]->isReady()) {
      selectedNodes[i]->apply();
    }
  }

  graphStateChanged();
  LogWidget::instance()->setText("Selection pasted");
  updateGL();
}

void GraphWidget::graphToGremlin() {
  unsigned int nbSelection = 0;
  unsigned int graphNumber = 0;
  NodeInterface *node = NULL;
  for(unsigned int i=0;i<_pbGraph->nbNodes();++i) {
    if(nodeSelected(i)) {

      if(nbSelection==0) {
	node = _pbGraph->node(i);
	graphNumber = node->graphId();
      } else {
	if(_pbGraph->node(i)->graphId()!=graphNumber) {
	  LogWidget::instance()->setText("Please, select only one graph to group");
	  return;
	}
      }

      nbSelection++;
    }
  }
  
  if(!node) {
    LogWidget::instance()->setText("Unable to create the group: no node selected");
    return;
  }

  // detach all nodes from the widgets 
  for(unsigned int i=0;i<_pbGraph->nbNodes();++i) {
    if(_pbGraph->node(i)->graphId()==graphNumber) {
      _animationWidget->delParams(_pbGraph->node(i)->widget());
      _nodeInterfaceWidget->removeWidget(_pbGraph->node(i)->widget());
      _interactionWidget->removeNode(_pbGraph->node(i));

    }
  }

  Gremlin *gremlin = _pbGraph->connexPartToGremlin(node);
  if(gremlin) {
    unselectNodeOutputs();
    LogWidget::instance()->setText("Group created");
  } else {
    LogWidget::instance()->setText("Group creation failed");
    return;
  }

  _pbGraph->applyFrom(gremlin);
  graphStateChanged();
  updateGL();
}

void GraphWidget::gremlinToGraph() {
  NodeInterface *node = NULL;
  for(unsigned int i=0;i<_pbGraph->nbNodes();++i) {
    if(nodeSelected(i)) {
      if(!node)
	node = _pbGraph->node(i);
      else {
	LogWidget::instance()->setText("Please, select only one single node");
	return;
      }
    } 
  }

  // check if one node is selected
  if(!node) {
    return;
  }

  // check if the node is a gremlin
  if(!node->isGrouped()) {
    LogWidget::instance()->setText("This node is not grouped");
    return;
  }

  // check if the node is connected 
  PbGraph::AdjacencyMatrix m = _pbGraph->adjacencyMatrix();
  const unsigned int id = node->nodeId();
  for(unsigned int i=0;i<m.size();++i) {
    bool b1 = m[i][id].empty();
    bool b2 = m[id][i].empty();
    if(!b1 || !b2) {
      LogWidget::instance()->setText("Please, unconnect the node before extraction");
      return;
    }
  }

  Gremlin *gremlin = _pbGraph->gremlinToConnexPart((Gremlin *)node);

  if(!gremlin) {
    LogWidget::instance()->setText("Group extraction failed");
    return;
  } else {
    _animationWidget->delParams(gremlin->widget());
    _nodeInterfaceWidget->removeWidget(gremlin->widget());
    _interactionWidget->removeNode(gremlin);
    gremlin->cleanEverything();
    delete gremlin;
    unselectNodeOutputs();
    _pbGraph->applyAll();
    graphStateChanged();
    updateGL();
    LogWidget::instance()->setText("Group extracted");
  }
}

void GraphWidget::addNodeToList() {
  NodeInterface *node = NULL;
  for(unsigned int i=0;i<_pbGraph->nbNodes();++i) {
    if(nodeSelected(i)) {
      if(!node)
	node = _pbGraph->node(i);
      else {
	LogWidget::instance()->setText("Please, select only one single node");
	return;
      }
    } 
  }
  
  // check if one node is selected
  if(!node) {
    return;
  }

  CustomNodeWidget w(node,this);
  w.exec();
}

void GraphWidget::manageNodePaths() {
  ManagePathsWidget w;
  w.exec();
}

void GraphWidget::saveNodeContent() {
  NodeInterface *node = NULL;
  unsigned int output = 0;
  
  for(unsigned int i=0;i<_pbGraph->nbNodes();++i) {
    if(!_pbGraph->node(i)) continue;
    for(unsigned int j=0;j<_pbGraph->node(i)->nbOutputs();++j) {
      if(!_pbGraph->node(i)->output(j)) continue;
      
      if(_pbGraph->node(i)->output(j)->selected()) {
  	if(!node) {
  	  node = _pbGraph->node(i);
  	  output = j;
  	}else {
  	  LogWidget::instance()->setText("Please, select only one single node output");
  	  return;
  	}
      
      }
    }
  }

    // check if one node is selected
    if(!node) {
      return;
    }
    
    QString filename = node->prepareToExportOutputContent(output);

    if(filename.isNull() || filename.isEmpty()) return;

    QFile f(filename);
    if(f.exists()) {
      QString text = filename+" already exist! Do you want to replace it?";
      QMessageBox::StandardButton ret;
      ret = QMessageBox::warning(this,tr("File exists"),text,
  				 QMessageBox::Save | QMessageBox::Cancel);
      if(ret==QMessageBox::Cancel)
  	return;
    }
    
    node->exportOutputContent(filename,output);

    LogWidget::instance()->setText(filename+" saved!");
}

void GraphWidget::saveAnimNodeContent() {
  NodeInterface *node = NULL;
  unsigned int output = 0;
  
  for(unsigned int i=0;i<_pbGraph->nbNodes();++i) {
    if(!_pbGraph->node(i)) continue;
    for(unsigned int j=0;j<_pbGraph->node(i)->nbOutputs();++j) {
      if(!_pbGraph->node(i)->output(j)) continue;
      
      if(_pbGraph->node(i)->output(j)->selected()) {
  	if(!node) {
  	  node = _pbGraph->node(i);
  	  output = j;
  	}else {
  	  LogWidget::instance()->setText("Please, select only one single node output");
  	  return;
  	}
      
      }
    }
  }

    // check if one node is selected
    if(!node) {
      return;
    }
    
    QString filename = node->prepareToExportOutputContent(output);

    if(filename.isNull() || filename.isEmpty()) return;

    unsigned int firstFrame = _animation->firstFrame();
    unsigned int lastFrame = _animation->lastFrame();
    unsigned int zeros = (unsigned int)ceil(log10((double)lastFrame));

    QStringList p = filename.split("/");
    QString name = p[p.size()-1];
    QString path;
    for(int i=0;i<p.size()-1;++i) {
      path = path + p[i] + "/";
    }

    char f[1024];
    unsigned int nbExistingFiles = 0;
    for(unsigned int i=firstFrame;i<=lastFrame;++i) {
      sprintf(f,"%s%0*d-%s", path.toStdString().c_str(),zeros, i,name.toStdString().c_str());
      QFile file(f);
      if(file.exists()) {
	nbExistingFiles++;
      }
    }

    if(nbExistingFiles>0) {
      QString text = " Do you want to replace " + QString::number(nbExistingFiles) + " files?";
      QMessageBox::StandardButton ret;
      ret = QMessageBox::warning(this,tr("File exists"),text,
    				 QMessageBox::Save | QMessageBox::Cancel);
      if(ret==QMessageBox::Cancel)
    	return;
    }
    
    for(unsigned int i=firstFrame;i<=lastFrame;++i) {
      sprintf(f,"%s%0*d-%s", path.toStdString().c_str(),zeros, i,name.toStdString().c_str());
      _animation->setCurrentFrame(i);
      _animationWidget->animationFrameChanged();
      node->exportOutputContent(f,output);
      LogWidget::instance()->setText(QString(f)+" saved!");
      LogWidget::instance()->repaint();
      _animationWidget->repaint();
      _interactionWidget->repaint();
      _nodeListWidget->repaint();
      _nodeInterfaceWidget->repaint();
      updateGL();
    }
}


bool GraphWidget::save(SceneSaver  *saver) {
  bool ret = true;
  if(!saver->saveIOData  ("graph"    ,_pbGraph  )) ret = false;
  if(!saver->saveFloat   ("scale"    ,_scale    )) ret = false;
  if(!saver->saveVector2f("offset"   ,_offset   )) ret = false;
  if(!saver->saveIOData  ("animation",_animation)) ret = false;
  return ret;
}

bool GraphWidget::load(SceneLoader *loader) {
  bool ret = true;

  _interaction      = NO_MOTION;
  _clickState       = NOTHING_HAS_BEEN_CLICKED;
  _mouseCoord       = Vector2f(0,0);
  _mouseTmp         = Vector2f(0,0);
  _inNodeSelection  = Vector2i(-1,-1);
  _outNodeSelection = Vector2i(-1,-1);
  _ctrl             = false;
  _shift            = false;

  if(!loader->loadIOData  ("graph"    ,_pbGraph  )) ret = false;
  if(!loader->loadFloat   ("scale"    ,_scale    )) ret = false;
  if(!loader->loadVector2f("offset"   ,_offset   )) ret = false;
  if(!loader->loadIOData  ("animation",_animation)) ret = false;

  animationSettingsChanged();

  return ret;
}

void GraphWidget::removeFromInteractionWidgets(NodeInterface *node) {
  if(!node) return;
  _animationWidget->delParams(node->widget());
  _nodeInterfaceWidget->removeWidget(node->widget());
  _interactionWidget->removeNode(node);
}

void GraphWidget::viewAll() {
  if(_pbGraph->nbNodes()==0) return;

  Vector2f minpos = _pbGraph->node(0)->pos();
  Vector2f maxpos = _pbGraph->node(0)->pos()+_pbGraph->node(0)->size();

  for(unsigned int i=1;i<_pbGraph->nbNodes();++i) {
    const Vector2f tmpmin = _pbGraph->node(i)->pos();
    const Vector2f tmpmax = _pbGraph->node(i)->pos()+_pbGraph->node(i)->size();

    minpos[0] = min(minpos[0],tmpmin[0]);
    minpos[1] = min(minpos[1],tmpmin[1]);
    maxpos[0] = max(maxpos[0],tmpmax[0]);
    maxpos[1] = max(maxpos[1],tmpmax[1]);
  }
  
  Vector2f fullsize = maxpos-minpos+(maxpos-minpos)/20.0f;
  fullsize[0] = fabs(fullsize[0]);
  fullsize[1] = fabs(fullsize[1]);

  Vector2f scales(fullsize[0]/(float)width(),fullsize[1]/(float)height());

  _scale  = max(scales[0],scales[1]);
  _offset = minpos;

  Vector2f p1 = remapCoord(width()/2,height()/2);
  Vector2f p2 = remapCoord(0,0);
  Vector2f p3 = remapCoord(width(),height());
  Vector2f p4 = ((p3-p2)-fullsize)/2.0f;

  _offset = _offset+(p1-p2-p4);
  updateGL();
}
