// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "core/nodeManager.h"
#include "core/gremlin.h"

#include <iostream>
#include <QPluginLoader>
#include <QObject>
#include <QDir>
#include <QCoreApplication>

using namespace std;

const QString NodeManager::LIB_NODE_PATH    = "lib";
const QString NodeManager::CUSTOM_NODE_PATH = "resources/customs";
const QString NodeManager::NODE_PATHS_FILE  = "resources/config/gratin.paths";
NodeManager  *NodeManager::_instance        = NULL;

NodeManager::NodeManager()
  : _handles(map<QString,NodeHandle *>()),
    _customHandles(map<QString,pair<CustomNodeHandle *,OrigNodeHandle *> > ()) {
  loadNodePaths();
  loadCoreHandles();
  loadCustomHandles();
}

NodeManager::~NodeManager() {
  cleanCoreHandles();
  cleanCustomHandles();
}

const QStringList NodeManager::customNodePaths() {
  return QStringList() << QCoreApplication::applicationDirPath()+"/"+CUSTOM_NODE_PATH << _nodePaths;
}

const QStringList NodeManager::libNodePaths() {
  return QStringList() << QCoreApplication::applicationDirPath()+"/"+LIB_NODE_PATH << _nodePaths;
}

const QStringList NodeManager::nodePaths() {
  return _nodePaths;
}

void NodeManager::loadNodePaths() {
  _nodePaths.clear();

  QFile file(QCoreApplication::applicationDirPath()+"/"+NODE_PATHS_FILE);
  if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    return;
  }
  
  QTextStream in(&file);
  
  while(!in.atEnd()) {
    QString line = in.readLine();    
    if(!line.isEmpty()) {
      _nodePaths.push_back(line);
    }
  }
  
  file.close();
}


void NodeManager::setNodePaths(const QStringList &nodePaths) {
  QFile file(QCoreApplication::applicationDirPath()+"/"+NODE_PATHS_FILE);
  if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    return;
  }
  
  QTextStream out(&file);
  
  foreach(QString path,nodePaths) {
    out << path << "\n";
  }

  file.close();

  _nodePaths = nodePaths;
}

void NodeManager::loadCoreHandles() {
  const QStringList libPaths = libNodePaths();
  foreach(QString d,libPaths) {
    QDir pluginsDir(d);
    //QDir pluginsDir(QCoreApplication::applicationDirPath());
    //pluginsDir.cd(LIB_NODE_PATH);
    
    foreach(QString fileName, pluginsDir.entryList(QDir::Files)) {
      //cout << "file found : " << pluginsDir.absoluteFilePath(fileName).toStdString() << endl;
      QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
      QObject *plugin = loader.instance();
      if(plugin) {
	//cout << "is a plugin : " << endl;
	NodeHandle *handle = qobject_cast<NodeHandle *>(plugin);
	if(handle) {
	  _handles[handle->name()] = handle;
	} else {
	  cout << "Unable to load " << fileName.toStdString() << endl;
	}
      }
    }
  }
}

void NodeManager::loadCustomHandles() {
  const QStringList customPaths = customNodePaths();
  foreach(QString d,customPaths) {
    //QDir customsDir(QCoreApplication::applicationDirPath());
    //customsDir.cd(CUSTOM_NODE_PATH);
    QDir customsDir(d);
    foreach(QString fileName, customsDir.entryList(QDir::Files)) {
      QString absfilename = customsDir.absoluteFilePath(fileName);
      SceneLoader *loader = initNodeLoading(absfilename);
      if(!loader) {
	continue;
      }
      
      // loading only handle
      QString cname("customNode"),cpath("custom/"),cdesc,chelp;
      QStringList cinputDesc,coutputDesc;
      int cinputDescSize=0,coutputDescSize=0;
      if(!loader->loadString("customHandleName",cname) ||
	 !loader->loadString("customHandlePath",cpath)) {
	delete loader;
	continue;
      }
      
      loader->loadString("customHandleDesc",cdesc);
      loader->loadString("customHandleHelp",chelp);
      loader->loadInt("customHandleInputDescSize",cinputDescSize);
      loader->loadInt("customHandleOutputDescSize",coutputDescSize);
      
      for(int i=0;i<cinputDescSize;++i) {
	QString currentDesc;
	loader->loadString(Tags::attribId("customHandleInputDesc",i),currentDesc);
	cinputDesc.push_back(currentDesc);
      }
      
      for(int i=0;i<coutputDescSize;++i) {
	QString currentDesc;
	loader->loadString(Tags::attribId("customHandleOutputDesc",i),currentDesc);
	coutputDesc.push_back(currentDesc);
      }
      
      
      // loading only handle
      QString oname("untitledNode"),opath("untitled/"),odesc,ohelp;
      QStringList oinputDesc,ooutputDesc;
      int oinputDescSize=0,ooutputDescSize=0;
      if(!loader->loadString("origHandleName",oname) ||
	 !loader->loadString("origHandlePath",opath)) {
	delete loader;
	continue;
      }
      
      loader->loadString("origHandleDesc",odesc);
      loader->loadString("origHandleHelp",ohelp);
      loader->loadInt("origHandleInputDescSize",oinputDescSize);
      loader->loadInt("origHandleOutputDescSize",ooutputDescSize);
      
      for(int i=0;i<oinputDescSize;++i) {
	QString currentDesc;
	loader->loadString(Tags::attribId("origHandleInputDesc",i),currentDesc);
	oinputDesc.push_back(currentDesc);
      }
      
      for(int i=0;i<ooutputDescSize;++i) {
	QString currentDesc;
	loader->loadString(Tags::attribId("origHandleOutputDesc",i),currentDesc);
	ooutputDesc.push_back(currentDesc);
      }
      
      CustomNodeHandle *chandle = new CustomNodeHandle(absfilename,cname,cpath,cdesc,chelp,cinputDesc,coutputDesc);
      OrigNodeHandle *ohandle = new OrigNodeHandle(oname,opath,odesc,ohelp,oinputDesc,ooutputDesc);
      
      _customHandles[chandle->name()] = pair<CustomNodeHandle *,OrigNodeHandle *>(chandle,ohandle);
      
      delete loader; loader = NULL;
    }
  }
}

void NodeManager::cleanCoreHandles() {
  map<QString,NodeHandle *>::const_iterator mit(_handles.begin()),mend(_handles.end());
  
  for(;mit!=mend;++mit){
    delete mit->second;
  }
  _handles.clear();
}

void NodeManager::cleanCustomHandles() {
  map<QString,pair<CustomNodeHandle *,OrigNodeHandle *> >::const_iterator mit(_customHandles.begin()),mend(_customHandles.end());
  
  for(;mit!=mend;++mit){
    delete mit->second.first;
    delete mit->second.second;
  }
  _customHandles.clear();
}

SceneLoader *NodeManager::initNodeLoading(const QString &filename) {
  QFile file(filename);

  if(!file.open(QFile::ReadOnly | QFile::Text)) return NULL;
  SceneLoader *loader = new SceneLoader();
  
  if(!loader->readFrom(file) || !loader->init("gratin","file","customNode")) {
    delete loader;
    return NULL;
  }

  return loader;
}


bool NodeManager::saveCustomNode(CustomNodeHandle *handle,NodeInterface *node) {
  QFile file(handle->filename());
  
  if(!file.open(QFile::WriteOnly | QFile::Text)) return false;
  
  SceneSaver *saver = new SceneSaver();
  if(!saver->init("gratin","file","customNode")) {
    delete saver;
    return false;
  }

  bool ret = true;

  QStringList inputs  = handle->inputDesc();
  QStringList outputs = handle->outputDesc();

  if(!saver->saveString("customHandleName",handle->name()))        ret = false;
  if(!saver->saveString("customHandlePath",handle->path()))        ret = false;
  if(!saver->saveString("customHandleDesc",handle->desc()))        ret = false;
  if(!saver->saveString("customHandleHelp",handle->help()))        ret = false;
  if(!saver->saveInt("customHandleInputDescSize",inputs.size()))   ret = false;
  if(!saver->saveInt("customHandleOutputDescSize",outputs.size())) ret = false;
    
  for(int i=0;i<inputs.size();++i) {
    if(!saver->saveString(Tags::attribId("customHandleInputDesc",i),inputs[i])) 
      ret = false;
  }
    
  for(int i=0;i<outputs.size();++i) {
    if(!saver->saveString(Tags::attribId("customHandleOutputDesc",i),outputs[i])) 
      ret = false;
  }

  inputs = node->inputDesc();
  outputs = node->outputDesc();

  if(!saver->saveString("origHandleName",node->name()))          ret = false;
  if(!saver->saveString("origHandlePath",node->path()))          ret = false;
  if(!saver->saveString("origHandleDesc",node->desc()))          ret = false;
  if(!saver->saveString("origHandleHelp",node->help()))          ret = false;
  if(!saver->saveInt("origHandleInputDescSize",inputs.size()))   ret = false;
  if(!saver->saveInt("origHandleOutputDescSize",outputs.size())) ret = false;
    
  for(int i=0;i<inputs.size();++i) {
    if(!saver->saveString(Tags::attribId("origHandleInputDesc",i),inputs[i])) 
      ret = false;
  }
    
  for(int i=0;i<outputs.size();++i) {
    if(!saver->saveString(Tags::attribId("origHandleOutputDesc",i),outputs[i])) 
      ret = false;
  }


  saveNode(saver,node,node->parent(),ret);
  saveNodeData(saver,node,ret);

  saver->writeTo(file);

  delete saver;
  return ret;
}


const vector<NodeHandle *> NodeManager::nodeHandles() const {
  vector<NodeHandle *> handles;

  map<QString,NodeHandle *>::const_iterator mit1(_handles.begin()),mend1(_handles.end());
  for(;mit1!=mend1;++mit1) {
    handles.push_back(mit1->second);
  }
  
  map<QString,pair<CustomNodeHandle *,OrigNodeHandle *> >::const_iterator mit2(_customHandles.begin()),mend2(_customHandles.end());
  for(;mit2!=mend2;++mit2) {
    handles.push_back(mit2->second.first);
  }

  return handles;
}

void NodeManager::saveNode(SceneSaver *saver,NodeInterface *node,PbGraph *,bool &ret,unsigned int id) {
  if(!saver->saveString  (Tags::attribId("nodeName"     ,id),node->name()     )) ret = false;
  if(!saver->saveString  (Tags::attribId("nodePath"     ,id),node->path()     )) ret = false;
  if(!saver->saveUint    (Tags::attribId("nodeId"       ,id),node->nodeId()   )) ret = false;
  if(!saver->saveUint    (Tags::attribId("nodeGraphId"  ,id),node->graphId()  )) ret = false;
  if(!saver->saveUint    (Tags::attribId("nbInputs"     ,id),node->nbInputs() )) ret = false;
  if(!saver->saveUint    (Tags::attribId("nbOutputs"    ,id),node->nbOutputs())) ret = false;
  if(!saver->saveVector2f(Tags::attribId("nodePos"      ,id),node->pos()      )) ret = false;
  if(!saver->saveBool    (Tags::attribId("nodeGrouped"  ,id),node->isGrouped())) ret = false;
}

void NodeManager::saveNodeData(SceneSaver *saver,NodeInterface *node,bool &ret,unsigned int id) {
  if(!saver->saveBool    (Tags::attribId("nodeHasWidget",id),node->widget())) ret = false;
  if(!saver->saveIOData  (Tags::attribId("node"         ,id),node          )) ret = false;
  if(node->widget()) 
    if(!saver->saveIOData(Tags::attribId("nodeWidget"   ,id),node->widget())) ret = false;


  for(unsigned int j=0;j<node->nbInputs();++j) {
    if(!saver->saveVector2f(Tags::attribId("nodeInputPos",id,j),node->inputPos(j))) 
      ret = false;
  }

  for(unsigned int j=0;j<node->nbOutputs();++j) {
    if(!saver->saveVector2f(Tags::attribId("nodeOutputPos",id,j),node->outputPos(j))) 
      ret = false;
  }
}

NodeInterface *NodeManager::loadNode(SceneLoader *loader,PbGraph *parent,bool &ret,unsigned int id,bool knownIO,unsigned int nbIn,unsigned int nbOut) {
  unsigned int nodeId=0,nodeGraphId=0,nbInputs=nbIn,nbOutputs=nbOut;
  QString      nodeName("noname"),nodePath("nopath");
  Vector2f     nodePos(0,0);
  bool         nodeGrouped=false;
  if(!loader->loadString  (Tags::attribId("nodeName"   ,id),nodeName   )) ret = false;
  if(!loader->loadString  (Tags::attribId("nodePath"   ,id),nodePath   )) ret = false;
  if(!loader->loadUint    (Tags::attribId("nodeId"     ,id),nodeId     )) ret = false;
  if(!loader->loadUint    (Tags::attribId("nodeGraphId",id),nodeGraphId)) ret = false;

  if(!knownIO || (knownIO && nbIn==0 && nbOut==0)) {
    if(!loader->loadUint    (Tags::attribId("nbInputs"   ,id),nbInputs   )) ret = false;
    if(!loader->loadUint    (Tags::attribId("nbOutputs"  ,id),nbOutputs  )) ret = false;
  }

  if(!loader->loadVector2f(Tags::attribId("nodePos"    ,id),nodePos    )) ret = false;
  if(!loader->loadBool    (Tags::attribId("nodeGrouped",id),nodeGrouped)) ret = false;
  
  if(!nodeGrouped) {
    // the node is part of the library - direct loading
    NodeInterface *n = createInstance(nodeName,parent,nbInputs,nbOutputs);
    if(!n) {
      n = new DefaultNode(parent,nbInputs,nbOutputs);
      ret = false;
    }
    n->setNodeId(nodeId);
    n->setGraphId(nodeGraphId);
    n->setPos(nodePos);
    return n;

  } else {
    // the node is grouped - load nodes inside the group
    Gremlin *g = new Gremlin(parent);
    g->setIoMode(Gremlin::IO_GREMLIN);
    if(!loader->loadIOData(Tags::attribId("node",id),(NodeInterface *)g)) 
      ret = false;
    g->setIoMode(Gremlin::IO_USER_DATA);
    g->setNodeId(nodeId);
    g->setGraphId(nodeGraphId);
    g->setPos(nodePos);
    if(isCustomNode(nodeName)) {
      g->setHandle(_customHandles[nodeName].first);
    }

    return g;    
  } 

  return NULL;
}

void NodeManager::loadNodeData(SceneLoader *loader,NodeInterface *node,bool &ret,unsigned int id) {
  bool hasWidget = false;

  if(!loader->loadBool(Tags::attribId("nodeHasWidget",id),hasWidget))       ret = false;
  if(!loader->loadIOData(Tags::attribId("node",id),node))                   ret = false;
  if(hasWidget) {
    if(!loader->loadIOData(Tags::attribId("nodeWidget",id),node->widget())) ret = false;
  }
  for(unsigned int j=0;j<node->nbInputs();++j) {
    Vector2f p(0,0);
    if(!loader->loadVector2f(Tags::attribId("nodeInputPos",id,j),p)) ret = false;
    node->setInputPos(p,j);
  }

  for(unsigned int j=0;j<node->nbOutputs();++j) {
    Vector2f p(0,0);
    if(!loader->loadVector2f(Tags::attribId("nodeOutputPos",id,j),p)) ret = false;
    node->setOutputPos(p,j);
  }

  node->updatePositions();
}
 
NodeInterface *NodeManager::loadCustomNode(PbGraph *parent,CustomNodeHandle *handle,unsigned int nbInputs,unsigned int nbOutputs) {
  SceneLoader *loader = initNodeLoading(handle->filename());
  if(!loader) return NULL;
  
  bool ret = true;
  NodeInterface *node = loadNode(loader,parent,ret,0,true,nbInputs,nbOutputs);

  if(node) node->setHandle(handle);

  delete loader;
  
  return node;
}

void NodeManager::loadCustomNodeData(NodeInterface *node) {
  if(!node) return; 

  map<QString,pair<CustomNodeHandle *,OrigNodeHandle *> >::const_iterator i = _customHandles.find(node->name());
  if(i==_customHandles.end()) return;

  CustomNodeHandle *handle = i->second.first;

  SceneLoader *loader = initNodeLoading(handle->filename());
  if(!loader) return;

  bool ret = true;
  loadNodeData(loader,node,ret);

  delete loader;
}

NodeInterface *NodeManager::createInstance(const QString &name,PbGraph *parent,unsigned int nbInputs,unsigned int nbOutputs) const {
  map<QString,NodeHandle *>::const_iterator i = _handles.find(name);
  if(i!=_handles.end()) return i->second->createInstance(parent,nbInputs,nbOutputs);
  
  map<QString,pair<CustomNodeHandle *,OrigNodeHandle *> >::const_iterator j = _customHandles.find(name);
  if(j!=_customHandles.end()) return j->second.first->createInstance(parent,nbInputs,nbOutputs);
  
  return NULL;
}

NodeHandle *NodeManager::createCustomNode(NodeInterface *node,const QString &name,const QString &path,
					  const QString &desc,const QString &help,
					  const QStringList &inputDesc,const QStringList &outputDesc,
					  const QString &dir) {

  CustomNodeHandle *chandle  = NULL;
  OrigNodeHandle   *ohandle  = NULL;

  map<QString,pair<CustomNodeHandle *,OrigNodeHandle *> >::const_iterator i,j;
  
  // get/create the new node handle 
  i = _customHandles.find(name);
  if(i!=_customHandles.end()) {
    // Check if we simply replace an existing handle 
    chandle = i->second.first;
    ohandle = i->second.second;
    chandle->setFilename(dir+"/"+name+".gratin-custom");
  } else {
    // create a new handle 
    QString filename(dir+"/"+name+".gratin-custom");
    chandle = new CustomNodeHandle(filename);
    ohandle = new OrigNodeHandle();
  } 
  
  // get/create the original node handle 
  j = _customHandles.find(node->name());
  if(j!=_customHandles.end()) {
    // Check if we simply replace an existing handle 
    OrigNodeHandle *h = j->second.second;
    ohandle->setName(h->name());
    ohandle->setPath(h->path());
    ohandle->setDesc(h->desc());
    ohandle->setHelp(h->help());
    ohandle->setInputDesc(h->inputDesc());
    ohandle->setOutputDesc(h->outputDesc());
  } else {
    // create a new handle 
    ohandle->setName(node->name());
    ohandle->setPath(node->path());
    ohandle->setDesc(node->desc());
    ohandle->setHelp(node->help());
    ohandle->setInputDesc(node->inputDesc());
    ohandle->setOutputDesc(node->outputDesc());
  } 
  
  chandle->setName(name);
  chandle->setPath(path);
  chandle->setDesc(desc);
  chandle->setHelp(help);
  chandle->setInputDesc(inputDesc);
  chandle->setOutputDesc(outputDesc);

  // set handle 
  _customHandles[chandle->name()] = pair<CustomNodeHandle *,OrigNodeHandle *>(chandle,ohandle);

  node->setHandle(ohandle);
  saveCustomNode(chandle,node);
  node->setHandle(chandle);

  return chandle;
}

bool NodeManager::isCoreNode(const QString &name) const {
  map<QString,NodeHandle *>::const_iterator i = _handles.find(name);
  return i!=_handles.end();
}

bool NodeManager::isCustomNode(const QString &name) const {
  map<QString,pair<CustomNodeHandle *,OrigNodeHandle *> >::const_iterator j = _customHandles.find(name);
  return j!=_customHandles.end();
}

NodeHandle *NodeManager::handle(const QString &name) {
  map<QString,NodeHandle *>::const_iterator i = _handles.find(name);
  if(i!=_handles.end()) return i->second;
  
  map<QString,pair<CustomNodeHandle *,OrigNodeHandle *> >::const_iterator j = _customHandles.find(name);
  if(j!=_customHandles.end()) return j->second.first;

  return NULL;
}
