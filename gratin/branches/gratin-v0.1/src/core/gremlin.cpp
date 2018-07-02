// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "gremlin.h"
#include "core/nodeManager.h"

#include <QVBoxLayout>
using namespace std;

GremlinWidget::GremlinWidget(NodeInterface *node) 
  : NodeWidget(node),
    _t(new QTabWidget()),
    _keyframed(false) {
  
  QVBoxLayout *l = new QVBoxLayout();
  l->addWidget(_t);
  setLayout(l);
}

GremlinWidget::~GremlinWidget() {
  _t->setParent(0);
}

void GremlinWidget::addTab(NodeWidget *page,const QString &label) {
  if(!page) return;

  _t->addTab((QWidget *)page,label);

  if(page->isKeyframed()) 
    _keyframed = true;

  addChildWidget(page);

}

void GremlinWidget::clear() {
  _t->clear();
}



Gremlin::Gremlin(PbGraph *parent,NodeHandle *handle)
  : PbGraph(),
    NodeInterface(parent,handle),
    _locked(false),
    _nbInputs(0),
    _nbOutputs(0),
    _mode(0),
    _w(new GremlinWidget(this)) {

}

Gremlin::~Gremlin() {

}

bool Gremlin::addNode(NodeInterface *node) {
  if(_locked) return false;
  return PbGraph::addNode(node);
}

bool Gremlin::removeNode(NodeInterface *node,bool clean) {
  if(_locked) return false;
  return PbGraph::removeNode(node,clean);
}

bool Gremlin::makeConnection(NodeInterface *src,IONodeID outputId,NodeInterface *dst,IONodeID inputId) {
    if(_locked) return false;
    return PbGraph::makeConnection(src,outputId,dst,inputId);
}

bool Gremlin::removeConnection(NodeInterface *src,IONodeID outputId,NodeInterface *dst,IONodeID inputId) {
    if(_locked) return false;
    return PbGraph::removeConnection(src,outputId,dst,inputId);
}

const vector<pair<NodeInterface *,Vector2u> > Gremlin::inputNodeData(NodeID nodeId) {
  vector<pair<NodeInterface *,Vector2u> > idataG = PbGraph::inputNodeData(nodeId); // gremlin connections
  vector<pair<NodeInterface *,Vector2u> > idataP = _parent->inputNodeData(_nodeId);// parent connections

  for(unsigned int i=0;i<_inodes.size();++i) {
    // look for this node in the input node list
    if(_inodes[i]->nodeId()!=nodeId) continue;

    // internal input connection for this node 
    const unsigned int input = _iconnect[i];
    
    // look if one of the external output connection corresponds... 
    for(unsigned int j=0;j<idataP.size();++j) {
      const Vector2u c1 = idataP[j].second;
      if(c1[1]==i) { // ... to the current external  input connection 
	// create the mapping (with the internal node input connection)
	const Vector2u c2(c1[0],input); 
	idataG.push_back(pair<NodeInterface *,Vector2u>(idataP[j].first,c2));
	break;
      }
    }
  }
  
  return idataG;
}

void Gremlin::lock() {
  _locked=true;
  initConnections();
  initWidget();
  updatePositions();
}

void Gremlin::unlock() {
  _locked=false;
  clearConnections();
  clearWidget();
}

// void Gremlin::initDefaultPositions() {

// }

void Gremlin::updatePositions() {
  for(unsigned int i=0;i<nbNodes();++i) {
    node(i)->updatePositions();
  }
  
  NodeInterface::updatePositions();
}

void Gremlin::initEmptyData(const Vector2f &pos) {
  cleanData();
  _pos = pos;

  for(unsigned int i=0;i<nbNodes();++i) {
    node(i)->initEmptyData(node(i)->pos());
  }

  initDefaultPositions();
}

void Gremlin::cleanData() {
  _outputData.clear();
  _inputData.clear();
}

void Gremlin::cleanEverything() {

  unlock();
  for(unsigned int i=0;i<nbNodes();++i) {
    node(i)->cleanEverything();
    delete node(i);
  }

  cleanData();
  delete _w;_w = NULL;
}

void Gremlin::setInputData(const vector<pair<NodeInterface *,Vector2u> > &) {
  prepareForProcessing(false,false,false);
  _state = nbNodes()>0 ? node(0)->state() : NO_CHANGE;
  _ready = true;
  for(unsigned int i=0;i<nbNodes();++i) {
    if(!node(i)->isReady()) {
      _ready = false;
      return;
    }
  }

  if(isReady())
    updatePositions();
}

void Gremlin::reloadInputData() {
  NodeInterface::reloadInputData();
  for(unsigned int i=0;i<nbNodes();++i) {
    node(i)->reloadInputData();
  }
}

void Gremlin::initOutputData() {
  for(unsigned int i=0;i<nbNodes();++i) {
    node(i)->initOutputData();
  }

  updatePositions();
}

void Gremlin::cleanOutputData() {
  for(unsigned int i=0;i<nbNodes();++i) {
    node(i)->cleanOutputData();
  }
}

bool Gremlin::save(SceneSaver  *saver) {
  bool ret = true;
  if(!saver->saveUint("nbInputs",_nbInputs))   ret = false;
  if(!saver->saveUint("nbOutputs",_nbOutputs)) ret = false;
  if(!PbGraph::save(saver))                    ret = false;
  return ret;
}

bool Gremlin::load(SceneLoader *loader) {
  bool ret = true;

  if(_mode==IO_GREMLIN) {
    unlock();
    
    if(!loader->loadUint("nbInputs",_nbInputs))   ret = false;
    if(!loader->loadUint("nbOutputs",_nbOutputs)) ret = false;
    if(!loadNodeParams(loader))                   ret = false;
    if(!loadNodeConnections(loader))              ret = false;
    
    lock();

  } else {
    prepareForProcessing(false,false);
    if(!loadNodeData(loader)) ret = false;
  }
  
  return ret;
}

void Gremlin::initWidget() {
  _w->clear();
  for(unsigned int i=0;i<nbNodes();++i) {
    if(node(i)->widget()) {
      _w->addTab(node(i)->widget(),node(i)->nameId());
    }
  }
}

void Gremlin::clearWidget() {
  _w->clear();
}

void Gremlin::initConnections() {
  clearConnections();
  initInputConnections();
  initOutputConnections();
}

void Gremlin::initInputConnections() {
  for(unsigned int i=0;i<nbNodes();++i) {
    if(node(i)->nbInputs()==0) continue;
    
    vector<bool> connected(node(i)->nbInputs(),false);
    
    for(unsigned int j=0;j<nbNodes();++j) {
      for(unsigned int k=0;k<_adjacencyMatrix[j][i].size();++k) {
	connected[_adjacencyMatrix[j][i][k][1]] = true;
      }
    }

    for(unsigned int j=0;j<node(i)->nbInputs();++j) {
      if(!connected[j]) {
	_inodes.push_back(node(i));
	_iconnect.push_back(j);
      }
    }
  }

  _nbInputs = _inodes.size();
}

void Gremlin::initOutputConnections() {
  for(unsigned int i=0;i<nbNodes();++i) {
    if(node(i)->nbOutputs()==0) continue;
    
    vector<bool> connected(node(i)->nbOutputs(),false);
    
    for(unsigned int j=0;j<nbNodes();++j) {
      for(unsigned int k=0;k<_adjacencyMatrix[i][j].size();++k) {
	connected[_adjacencyMatrix[i][j][k][0]] = true;
      }
    }

    for(unsigned int j=0;j<node(i)->nbOutputs();++j) {
      if(!connected[j]) {
	_onodes.push_back(node(i));
	_oconnect.push_back(j);
      }
    }
  }

  _nbOutputs = _onodes.size();
}



void Gremlin::clearConnections() {
  _inodes.clear();
  _onodes.clear();
  _iconnect.clear();
  _oconnect.clear();
}

void Gremlin::getConnections(vector<pair<NodeInterface *,NodeInterface *> > &connectedNodes,
                             vector<Vector2u> &connectedOutputsInputs) const {
  connectedNodes.clear();
  connectedOutputsInputs.clear();
  for(NodeID i=0;i<nbNodes();++i) {
      for(unsigned int j=0;j<nbNodes();++j) {
          for(unsigned int k=0;k<_adjacencyMatrix[i][j].size();++k) {
              connectedNodes.push_back(pair<NodeInterface *,NodeInterface *>(_topologicalList[i],_topologicalList[j]));
              connectedOutputsInputs.push_back(_adjacencyMatrix[i][j][k]);
          }
      }
  }
}

const QStringList Gremlin::graphNames() {
  QStringList l = _parent->graphNames();
  l << nameId();
  return l;
}
