// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef GREMLIN_H
#define GREMLIN_H

#include "core/pbgraph.h"
#include "core/nodeManager.h"
#include "core/nodeInterface.h"
#include "view/nodeWidget.h"
#include <QTabWidget>

class GremlinWidget : public NodeWidget {
 public:
  GremlinWidget(NodeInterface *node);
  ~GremlinWidget();

  inline bool isKeyframed() const {return _keyframed;}
  void addTab(NodeWidget *page,const QString &label);
  void clear();

 private:
  QTabWidget *_t;
  bool _keyframed;
};


class Gremlin : public PbGraph, public NodeInterface {
  friend class PbGraph;
  friend class NodeManager;

public:
  Gremlin(PbGraph *parent=NULL,NodeHandle *handle=NULL);
  virtual ~Gremlin();

  // **************************************
  // ****** PART SPECIFIC TO PBGRAPH ******
  // **************************************
 public:
  virtual bool addNode         (NodeInterface *node);
  virtual bool removeNode      (NodeInterface *node,bool clean=true);
  virtual bool makeConnection  (NodeInterface *src,IONodeID outputId,
				NodeInterface *dst,IONodeID inputId);
  virtual bool removeConnection(NodeInterface *src,IONodeID outputId,
				NodeInterface *dst,IONodeID inputId);

  // remove all gremlin functionnalities in the gremlin class
  virtual Gremlin *connexPartToGremlin(NodeInterface *) {return NULL;}
  virtual Gremlin *gremlinToConnexPart(Gremlin *)       {return NULL;}

  // send events between nodes 
  inline virtual void sendEventFrom (NodeInterface *node,const NodeEvent &event);
  inline virtual void sendEventAll(const NodeEvent &event);

  // applying funtions 
  inline virtual void applyFrom (NodeInterface *node);
  inline virtual void applyAll();
  inline virtual void applyAllWithCond(const NodeCondition &condition);

  // reloading
  inline virtual void reloadAll();
  inline virtual void refreshView();
  virtual const QStringList graphNames();

  virtual const std::vector<std::pair<NodeInterface *,Vector2u> > inputNodeData(NodeID nodeId);





  // *********************************************
  // ****** PART SPECIFIC TO NODE INTERFACE ******
  // *********************************************

 public:
  inline virtual unsigned int nbInputs()  const;
  inline virtual unsigned int nbOutputs() const;
  inline virtual void apply ();
  inline virtual void reload();
  inline virtual void init  ();
  inline virtual void clean ();
  inline virtual NodeWidget *widget();
  inline virtual void keyPressEvent    (QKeyEvent *          );
  inline virtual void mousePressEvent  (const Vector2f &,QMouseEvent *);
  inline virtual void mouseMoveEvent   (const Vector2f &,QMouseEvent *);
  inline virtual void mouseReleaseEvent(const Vector2f &,QMouseEvent *);
  inline virtual void wheelEvent       (const Vector2f &,QWheelEvent *);
  inline virtual void receiveEvent(const NodeEvent &event);
  inline virtual bool satisfy(const NodeCondition &condition);
  inline virtual const QString     name();
  inline virtual const QString     path();
  inline virtual const QString     desc();
  inline virtual const QString     help();
  inline virtual const QStringList inputDesc();
  inline virtual const QStringList outputDesc();
  //inline virtual bool isReady() const {return _ready;}
  inline virtual bool checkDataType(NodeInterface *inputNode,
  				    unsigned int outputId,unsigned int inputId);
  //inline virtual bool checkInputData(const std::vector<Data *> &data) const;
  //inline virtual void draw(const Matrix4f &mvp=Matrix4f::Identity());
  inline virtual void drawOutputContent(const Matrix4f &mvp=Matrix4f::Identity(),
  					const Vector2f &position=Vector2f(0,0),unsigned int i=0);
  inline virtual const Vector2f outputContentSize(unsigned int i);
  inline virtual Data *input (unsigned int i) const;
  inline virtual Data *output(unsigned int i) const;
  inline virtual const std::vector<Data *> &inputs ();
  inline virtual const std::vector<Data *> &outputs();

  inline virtual bool isGrouped() const {return true;}

  //virtual void initDefaultPositions();
  virtual void updatePositions();
  virtual void initEmptyData(const Vector2f &pos=Vector2f(0,0));
  virtual void cleanData();
  virtual void cleanEverything();
  virtual void setInputData(const std::vector<std::pair<NodeInterface *,
			    Vector2u> > &dinputs);
  //virtual void setInputData(const std::vector<Data *> &newData);
  virtual void reloadInputData();
  virtual void initOutputData();
  virtual void cleanOutputData();

  virtual bool save(SceneSaver  *);
  virtual bool load(SceneLoader *);


  // **************************************
  // ****** PART SPECIFIC TO GREMLIN ******
  // **************************************
 public:
  void lock();
  void unlock();
  void getConnections(std::vector<std::pair<NodeInterface *,NodeInterface *> > &connectedNodes,
		      std::vector<Vector2u> &connectedOutputsInputs) const; 
 private:
  enum{IO_GREMLIN=0,IO_USER_DATA=1};

  void initConnections();
  void clearConnections();
  void initInputConnections();
  void initOutputConnections();

  void initWidget();
  void clearWidget();

  inline void setIoMode(int mode=0) {_mode = mode;}

  bool         _locked;
  unsigned int _nbInputs;
  unsigned int _nbOutputs;
  unsigned int _mode;

  std::vector<NodeInterface *> _inodes;
  std::vector<NodeInterface *> _onodes;
  std::vector<unsigned int>    _iconnect;
  std::vector<unsigned int>    _oconnect;

  GremlinWidget  *_w;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

inline unsigned int Gremlin::nbInputs()  const {
  return _nbInputs;
}

inline unsigned int Gremlin::nbOutputs() const {
  return _nbOutputs;
}

inline Data *Gremlin::input(unsigned int i) const {
  return _inodes[i]->input(_iconnect[i]);
}

inline Data *Gremlin::output(unsigned int i) const {
  return _onodes[i]->output(_oconnect[i]);
}

inline const std::vector<Data *> &Gremlin::inputs () {
  _inputData = std::vector<Data *>();
  for(unsigned int i=0;i<nbInputs();++i) {
    _inputData.push_back(input(i));
  }
  return NodeInterface::inputs();
}

inline const std::vector<Data *> &Gremlin::outputs() {
  _outputData = std::vector<Data *>();
  for(unsigned int i=0;i<nbOutputs();++i) {
    _outputData.push_back(output(i));
  }
  return NodeInterface::outputs();
}

inline void Gremlin::apply () {
  for(unsigned int i=0;i<nbNodes();++i) {
    node(i)->apply();
  }
}

inline void Gremlin::reload() {
  for(unsigned int i=0;i<nbNodes();++i) {
    node(i)->reload();
  }
}

inline void Gremlin::init  () {
  for(unsigned int i=0;i<nbNodes();++i) {
    node(i)->init();
  }
}

inline void Gremlin::clean () {
  for(unsigned int i=0;i<nbNodes();++i) {
    node(i)->clean();
  }
}

inline NodeWidget *Gremlin::widget() {
  return _w;
}

inline void Gremlin::keyPressEvent(QKeyEvent *ke) {
  for(unsigned int i=0;i<nbNodes();++i) {
    node(i)->keyPressEvent(ke);
  }
}

inline void Gremlin::mousePressEvent(const Vector2f &p,QMouseEvent *me) {
  for(unsigned int i=0;i<nbNodes();++i) {
    node(i)->mousePressEvent(p,me);
  }
}

inline void Gremlin::mouseMoveEvent(const Vector2f &p,QMouseEvent *me) {
  for(unsigned int i=0;i<nbNodes();++i) {
    node(i)->mouseMoveEvent(p,me);
  }
}

inline void Gremlin::mouseReleaseEvent(const Vector2f &p,QMouseEvent *me) {
  for(unsigned int i=0;i<nbNodes();++i) {
    node(i)->mouseReleaseEvent(p,me);
  }
}

inline void Gremlin::wheelEvent(const Vector2f &p,QWheelEvent *me) {
  for(unsigned int i=0;i<nbNodes();++i) {
    node(i)->wheelEvent(p,me);
  }
}

inline void Gremlin::receiveEvent(const NodeEvent &event) {
  for(unsigned int i=0;i<nbNodes();++i) {
    node(i)->receiveEvent(event);
  }
}

inline bool Gremlin::satisfy(const NodeCondition &condition) {
  bool s = false;
  for(unsigned int i=0;i<nbNodes();++i) {
    if(node(i)->satisfy(condition))
      s = true;
  }
  return s;
}

inline const QString Gremlin::name() {
  return _handle ? _handle->name() : QString("Group");
}

inline const QString Gremlin::path() {
  return _handle ? _handle->path() : QString("Groups/");
}

inline const QString Gremlin::desc() {
  return _handle ? _handle->desc() : QString("Group");
}

inline const QString Gremlin::help() {
  return _handle ? _handle->help() : QString("No help defined for this node");
}

#include <iostream>
using namespace std;
inline const QStringList Gremlin::inputDesc() {
  if(_handle) return _handle->inputDesc();

  QStringList l;
  for(unsigned int i=0;i<_nbInputs;++i) {
    cout << __LINE__ << endl;
    const QStringList tmp = _inodes[i]->inputDesc();
    cout << __LINE__ << " looking for " << _iconnect[i] << " in an array of size " << tmp.size() << endl;

    if(_iconnect[i]<tmp.size())
      l.push_back(tmp[_iconnect[i]]);
    else 
      l.push_back("inputName");
    cout << __LINE__ << endl;
  }
  return l;
}


inline const QStringList Gremlin::outputDesc() {
  if(_handle) return _handle->outputDesc();

  QStringList l;
  for(unsigned int i=0;i<_nbOutputs;++i) {
    const QStringList tmp = _onodes[i]->outputDesc();
    l.push_back(tmp[_oconnect[i]]);
  }
  return l;
}

//inline bool Gremlin::isReady() const {return _ready;}
inline bool Gremlin::checkDataType(NodeInterface *inputNode,
			  unsigned int outputId,unsigned int inputId) {
  return _inodes[inputId]->checkDataType(inputNode,outputId,_iconnect[inputId]);
}

/* inline bool Gremlin::checkInputData(const std::vector<Data *> &data) const { */
/*   return false; */
/* } */

/* inline void Gremlin::draw(const Matrix4f &mvp) { */
/*   NodeInterface::draw(mvp); */

/*   if(!isReady()) return; */

/*   for(unsigned int i=0;i<nbOutputs();++i) { */
/*     const Vector2f o = (outputSize(i)-outputContentSize(i))/2.0f; */
/*     drawOutputContent(mvp,_pos+outputPos(i)+o,i); */
/*   } */
/* } */

inline void Gremlin::drawOutputContent(const Matrix4f &mvp,const Vector2f &position,unsigned int i) {
  _onodes[i]->drawOutputContent(mvp,position,_oconnect[i]);
}

inline const Vector2f Gremlin::outputContentSize(unsigned int i) {
  return _onodes[i]->outputContentSize(_oconnect[i]);
}

inline void Gremlin::sendEventFrom(NodeInterface *,const NodeEvent &event) {
  _parent->sendEventFrom(this,event);
}

inline void Gremlin::sendEventAll(const NodeEvent &event) {
  _parent->sendEventAll(event);
}

inline void Gremlin::applyFrom(NodeInterface *) {
  _parent->applyFrom(this);
}

inline void Gremlin::applyAll() {
  _parent->applyAll();
}

inline void Gremlin::applyAllWithCond(const NodeCondition &condition) {
  _parent->applyAllWithCond(condition);
}

inline void Gremlin::reloadAll() {
  _parent->reloadAll();
}

inline void Gremlin::refreshView() {
  _parent->refreshView();
}


#endif // GREMLIN_H
