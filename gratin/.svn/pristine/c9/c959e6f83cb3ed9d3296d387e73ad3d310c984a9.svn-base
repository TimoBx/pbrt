// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef NODE_LIST_WIDGET_H
#define NODE_LIST_WIDGET_H

#include "core/nodeHandle.h"
#include "core/sceneIOTags.h"

#include <QDockWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QStringList>

class GraphWidget;

class NodeListWidget : public QDockWidget, public IOData {
  Q_OBJECT

public:
  ~NodeListWidget();

  // this widget is a singleton
  static NodeListWidget *instance(GraphWidget *graphWidget=NULL,QWidget *parent=NULL) {
    if(!_instance) {
      _instance = new NodeListWidget(graphWidget,parent);
    }
    return _instance;
  } 

  void setNodeList(const std::vector<NodeHandle *> nodeHandles);
  void addNodeList(NodeHandle *handle);
  void delNodeList(NodeHandle *handle);

  // load and save 
  bool save(SceneSaver  *) {return true;}
  bool load(SceneLoader *) {return true;}

public slots:
  void addNodeClicked(QTreeWidgetItem *,int);

private:
  NodeListWidget(GraphWidget *graphWidget,QWidget *parent=0);
  static NodeListWidget *_instance;

  GraphWidget *_graphWidget;
  QTreeWidget *_tree;
  std::vector<NodeHandle *> _nodeHandles;

  void addNode(NodeHandle *handle);
  void cleanTree();
  void makeTree();
  void makeTreeRec(QTreeWidget *tree,QStringList &path,const QString &desc,QTreeWidgetItem *parent=NULL,bool stop=false);
  void deleteItem(QTreeWidgetItem *item);
};

#endif // NODE_LIST_WIDGET_H
