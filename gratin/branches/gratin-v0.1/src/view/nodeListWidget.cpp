// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "view/nodeListWidget.h"
#include "view/graphWidget.h"

#include <QStringList>

using namespace std;

NodeListWidget *NodeListWidget::_instance = NULL;

NodeListWidget::NodeListWidget(GraphWidget *graphWidget,QWidget *parent)
  : QDockWidget(parent),
    _graphWidget(graphWidget),
    _tree(new QTreeWidget()) {

  setWidget(_tree);
  setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
  connect(_tree,SIGNAL(itemDoubleClicked(QTreeWidgetItem *,int)),this,SLOT(addNodeClicked(QTreeWidgetItem *,int)));
}

NodeListWidget::~NodeListWidget() {
  cleanTree();
}

void NodeListWidget::setNodeList(const vector<NodeHandle *> nodeHandles) {
  cleanTree();
  _nodeHandles = nodeHandles;
  makeTree();
}


void NodeListWidget::cleanTree() {
  _tree->clear();
}

void NodeListWidget::makeTree() {

  for(unsigned int i=0;i<_nodeHandles.size();++i) {
    addNode(_nodeHandles[i]);
  }

  _tree->setHeaderLabels(QStringList() << "Name" << "Description");
}

void NodeListWidget::addNode(NodeHandle *handle) {
  // get name and path
  QString name = handle->name();
  QString path = handle->path();
  QString desc = handle->desc();
  
  // remove all spaces
  name.remove(QChar(' '));
  path.remove(QChar(' '));
  
  // check slash at the end of the path
  if(path.at(path.size()-1)!=QChar('/'))
    path = path.append("/");
  
  // build the whole path+name
  QString fullname = path+name;
  
  QStringList t = fullname.split("/");
  makeTreeRec(_tree,t,desc);
}

void NodeListWidget::addNodeList(NodeHandle *handle) {
  addNode(handle);
  _nodeHandles.push_back(handle);
}

void NodeListWidget::delNodeList(NodeHandle *handle) {
  if(!handle) return;

  bool found = false;
  for(unsigned int i=0;i<_nodeHandles.size();++i) {
    if(_nodeHandles[i]->name()==handle->name()) {
      _nodeHandles.erase(_nodeHandles.begin()+i);
      found = true;
      break;
    }
  }

  if(!found) return;

  QList<QTreeWidgetItem *> items = _tree->findItems(handle->name(),Qt::MatchFixedString | Qt::MatchRecursive);
  QTreeWidgetItem *item = NULL;

  for(int i=0;i<items.size();++i) {
    if(items[i]->childCount()==0) {
      item = items[i];
      break;
    }
  }
  
  if(!item) return;

  deleteItem(item);
}

void NodeListWidget::deleteItem(QTreeWidgetItem *item) {  
  // special case of top level items
  if(_tree->topLevelItem(_tree->indexOfTopLevelItem(item))==item) {
    QTreeWidgetItem *itemToDelete = _tree->takeTopLevelItem(_tree->indexOfTopLevelItem(item));
    delete itemToDelete;
    return;
  }

  // children
  QTreeWidgetItem *parent = item->parent();
  const int i = parent->indexOfChild(item);
  QTreeWidgetItem *itemToDelete = parent->takeChild(i);
  delete itemToDelete;

  // if the item has no brothers no need to keep its parent
  if(parent->childCount()==0) {
    deleteItem(parent);
  }
}

void NodeListWidget::makeTreeRec(QTreeWidget *tree,QStringList &path,const QString &desc,QTreeWidgetItem *parent,bool stop) {
  if(path.empty())
    return;

  QTreeWidgetItem *item;

  QList<QTreeWidgetItem *> items = QList<QTreeWidgetItem *>();
  if(!stop) items = tree->findItems(path[0],Qt::MatchExactly | Qt::MatchRecursive,0);

  if(!items.empty()) {
    item = items[0];
  } else {
    stop = true;
    item = new QTreeWidgetItem();
    item->setText(0,path[0]);

    if(parent) {
      parent->insertChild(parent->childCount(),item);
    } else {
      tree->addTopLevelItem(item);
    }
  }

  path.removeFirst();

  if(path.empty())
    item->setText(1,desc);

  makeTreeRec(tree,path,desc,item,stop);
}

void NodeListWidget::addNodeClicked(QTreeWidgetItem *item,int) {
  QString name = item->text(0);

  _graphWidget->addNode(name);
}

