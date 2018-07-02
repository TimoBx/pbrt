// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "view/customNodeWidget.h"
#include "view/graphWidget.h"
#include "view/nodeListWidget.h"
#include "core/nodeManager.h"

#include <QGridLayout>
#include <QMessageBox>
#include <QFileDialog>

CustomNodeWidget::CustomNodeWidget(NodeInterface *node,GraphWidget *graphWidget)
  : _node(node),
    _graphWidget(graphWidget) {

  QGridLayout *g = new QGridLayout();
  g->setSpacing(20);

  _title          = new QLabel("<font color='black' size='16'>Node information</font>");
  _nameLabel      = new QLabel("Name");
  _pathLabel      = new QLabel("Path");
  _descLabel      = new QLabel("Description");
  _helpLabel      = new QLabel("Help");
  _inputsLabel    = new QLabel("Inputs");
  _outputsLabel   = new QLabel("Outputs");
  _directoryLabel = new QLabel("Directory");

  _nameEdit = new QLineEdit();
  _pathEdit = new QLineEdit("custom/");
  _descEdit = new QLineEdit(_node->desc());
  _helpEdit = new QTextEdit(_node->help());

  _inputsEdit  = new QComboBox();
  _outputsEdit = new QComboBox();
  _directory   = new QComboBox();
  _inputsEdit->addItems(_node->inputDesc());
  _outputsEdit->addItems(_node->outputDesc());
  _directory->addItems(NodeManager::instance()->customNodePaths());
  _inputsEdit->setEditable(true);
  _outputsEdit->setEditable(true);

  _inputsEdit->setInsertPolicy(QComboBox::InsertAtCurrent);
  _outputsEdit->setInsertPolicy(QComboBox::InsertAtCurrent);
  _buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  _title->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

  g->addWidget(_title,0,0,1,2);
  g->addWidget(_nameLabel,1,0,1,1);
  g->addWidget(_nameEdit,1,1,1,1);
  g->addWidget(_pathLabel,2,0,1,1);
  g->addWidget(_pathEdit,2,1,1,1);
  g->addWidget(_descLabel,3,0,1,1);
  g->addWidget(_descEdit,3,1,1,1);
  g->addWidget(_inputsLabel,4,0,1,1);
  g->addWidget(_inputsEdit,4,1,1,1);
  g->addWidget(_outputsLabel,5,0,1,1);
  g->addWidget(_outputsEdit,5,1,1,1);
  g->addWidget(_helpLabel,6,0,1,1);
  g->addWidget(_helpEdit,6,1,1,1);
  g->addWidget(_directoryLabel,7,0,1,1);
  g->addWidget(_directory,7,1,1,1);
  g->addWidget(_buttons,8,0,1,2);

  setLayout(g);

  setWindowModality(Qt::WindowModal);

  connect(_buttons,SIGNAL(accepted()),this,SLOT(confirmClicked()));
  connect(_buttons,SIGNAL(rejected()),this,SLOT(cancelClicked()));
  connect(_inputsEdit,SIGNAL(editTextChanged(const QString &)),this,SLOT(inputTextChanged(const QString &)));
  connect(_outputsEdit,SIGNAL(editTextChanged(const QString &)),this,SLOT(outputTextChanged(const QString &)));

}

void CustomNodeWidget::inputTextChanged(const QString &text) {
  _inputsEdit->setItemText(_inputsEdit->currentIndex(),text);
}

void CustomNodeWidget::outputTextChanged(const QString &text) {
  _outputsEdit->setItemText(_outputsEdit->currentIndex(),text);
}

void CustomNodeWidget::confirmClicked() {
  QString name = _nameEdit->text();
  QString path = _pathEdit->text();

  // check name and path validity 
  if(name.isNull() || name.isEmpty()) {
    QMessageBox::information(this,"Name not valid","The node name should not be empty");
    return;
  }

  QStringList l = name.split(" ");
  if(l.size()>1) {
    QMessageBox::information(this,"Name not valid","The node name should not contain spaces");
    return;
  }

  l = path.split(" ");
  if(l.size()>1) {
    QMessageBox::information(this,"Path not valid","The node path should not contain spaces");
    return;
  }

  // check if this node already exists
  if(NodeManager::instance()->isCoreNode(name)) {
    QMessageBox::information(this,"Name not valid","This node name already exist");
    return;
  }

  const bool isCustom = NodeManager::instance()->isCustomNode(name);
  if(isCustom) {
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this,"Name already exist","This name already exist...\nReplacing it may cause problems in other nodes which have the same name.\nReplace it anyway (non-reversible action)?",
			       QMessageBox::Ok | QMessageBox::Cancel);
    if(ret==QMessageBox::Cancel) {
      return;
    }
  }

  // get the remaining information
  QString desc = _descEdit->text();
  QString help = _helpEdit->toPlainText();
  QStringList inputDesc;
  QStringList outputDesc;

  for(int i=0;i<_inputsEdit->count();++i) {
    inputDesc.push_back(_inputsEdit->itemText(i));
  }

  for(int i=0;i<_outputsEdit->count();++i) {
    outputDesc.push_back(_outputsEdit->itemText(i));
  }

  _graphWidget->removeFromInteractionWidgets(_node);

  if(isCustom) NodeListWidget::instance()->delNodeList(NodeManager::instance()->handle(name));
  NodeHandle *after  = NodeManager::instance()->createCustomNode(_node,name,path,desc,help,inputDesc,outputDesc,_directory->currentText());
  NodeListWidget::instance()->addNodeList(after);

  accept();
}

void CustomNodeWidget::cancelClicked() {
  reject();
}
 

ManagePathsWidget::ManagePathsWidget() {

  QGridLayout *g = new QGridLayout();
  g->setSpacing(20);

  _title      = new QLabel("<font color='black' size='16'>Manage node paths</font>");
  _ruleLabel  = new QLabel("Operations will be effective at the next start of Gratin");
  _pathsList  = new QListWidget();
  _addButton  = new QPushButton("Add path");
  _editButton = new QPushButton("Edit path");
  _delButton  = new QPushButton("Del path");
  _buttons    = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  _pathsList->addItems(NodeManager::instance()->nodePaths());
  _title->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  _ruleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

  QWidget *bwidget = new QWidget();
  QVBoxLayout *l = new QVBoxLayout();
  l->addWidget(_addButton);
  l->addWidget(_editButton);
  l->addWidget(_delButton);
  bwidget->setLayout(l);

  g->addWidget(_title,0,0,1,5);
  g->addWidget(_ruleLabel,1,0,1,5);
  g->addWidget(_pathsList,2,0,4,4);
  g->addWidget(bwidget,2,4,1,1);
  g->addWidget(_buttons,6,0,1,5);

  setLayout(g);

  setWindowModality(Qt::WindowModal);

  connect(_buttons,SIGNAL(accepted()),this,SLOT(confirmClicked()));
  connect(_buttons,SIGNAL(rejected()),this,SLOT(cancelClicked()));

  connect(_addButton,SIGNAL(clicked()),this,SLOT(addClicked()));
  connect(_delButton,SIGNAL(clicked()),this,SLOT(delClicked()));
  connect(_editButton,SIGNAL(clicked()),this,SLOT(editClicked()));
}

void ManagePathsWidget::confirmClicked() {
  QStringList paths;

  for(int i=0;i<_pathsList->count();++i) {
    paths.push_back(_pathsList->item(i)->text());
  }

  NodeManager::instance()->setNodePaths(paths);

  accept();
}

void ManagePathsWidget::cancelClicked() {
  reject();
}

void ManagePathsWidget::addClicked() {
  QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
						  QCoreApplication::applicationDirPath()+"/data",
						  QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  if(dir.isEmpty()) return;
  
  _pathsList->addItem(dir);			  
}

void ManagePathsWidget::editClicked() {
  if(!_pathsList->currentItem()) {
    return;
  }

  QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
						  _pathsList->currentItem()->text(),
						  QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  if(dir.isEmpty()) return;

  _pathsList->currentItem()->setText(dir);
}

void ManagePathsWidget::delClicked() {
  delete _pathsList->takeItem(_pathsList->currentRow());
}
