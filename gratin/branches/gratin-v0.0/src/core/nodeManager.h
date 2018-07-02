// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef NODE_MANAGER_H
#define NODE_MANAGER_H

#include "misc/glinclude.h"
#include "core/nodeHandle.h"
#include "core/nodeInterface.h"
#include "core/pbgraph.h"
#include "core/sceneLoader.h"
#include "core/sceneSaver.h"

#include <QString>
#include <vector>
#include <map>

class CustomNodeHandle;
class OrigNodeHandle;

class NodeManager {
  friend class CustomNodeHandle;
  friend class OrigNodeHandle;

public:
  ~NodeManager();

  static const QString LIB_NODE_PATH;    // ./lib
  static const QString CUSTOM_NODE_PATH; // ./resources/customs
  static const QString NODE_PATHS_FILE;  // ./resources/config/gratin.paths

  // this class is a singleton
  static NodeManager *instance() { if(!_instance) _instance = new NodeManager(); return _instance; } 

  const std::vector<NodeHandle *> nodeHandles() const;
  NodeInterface *createInstance(const QString &name,PbGraph *parent,unsigned int nbInputs=0,unsigned int nbOutputs=0) const;
  NodeHandle *createCustomNode(NodeInterface *node,const QString &name,const QString &path,
			       const QString &desc,const QString &help,
			       const QStringList &inputDesc,const QStringList &outputDesc,
			       const QString &dir=QCoreApplication::applicationDirPath()+"/"+CUSTOM_NODE_PATH);

  void saveNode(SceneSaver  *saver ,NodeInterface *node,PbGraph *parent,bool &ret,unsigned int id=0);
  void saveNodeData(SceneSaver  *saver,NodeInterface *node,bool &ret,unsigned int id=0);
  NodeInterface *loadNode(SceneLoader *loader,PbGraph *parent,bool &ret,unsigned int id=0,bool knownIO=false,unsigned int nbIn=0,unsigned int nbOut=0);
  void loadNodeData(SceneLoader *loader,NodeInterface *node,bool &ret,unsigned int id=0);
  void loadCustomNodeData(NodeInterface *node);

  bool isCoreNode(const QString &name) const;
  bool isCustomNode(const QString &name) const;
  NodeHandle *handle(const QString &name);

  const QStringList customNodePaths();
  const QStringList libNodePaths();
  const QStringList nodePaths();

  void setNodePaths(const QStringList &nodePaths);

private:
  NodeManager();
  SceneLoader *initNodeLoading(const QString &filename);

  void loadCoreHandles();
  void cleanCoreHandles();
  void loadCustomHandles();
  void cleanCustomHandles();
  void loadNodePaths();

  NodeInterface *loadCustomNode(PbGraph *parent,CustomNodeHandle *handle,unsigned int nbInputs,unsigned int nbOutputs);
  bool           saveCustomNode(CustomNodeHandle *handle,NodeInterface *node);

  
  std::map<QString,NodeHandle *>                                    _handles;
  std::map<QString,std::pair<CustomNodeHandle *,OrigNodeHandle *> > _customHandles;
 
  QStringList _nodePaths;

  static NodeManager *_instance;
};

// this class is used to replace the initial handle in case the user creates a custom node 
class CustomNodeHandle : public NodeHandle {

 public:
  CustomNodeHandle(const QString &filename,
		   const QString &hname=QString(),
		   const QString &hpath=QString(),
		   const QString &hdesc=QString(),
		   const QString &hhelp=QString(),
		   const QStringList &hinputDesc=QStringList(),
		   const QStringList &houtputDesc=QStringList()) 
    : _filename(filename),
    _name(hname),
    _path(hpath),
    _desc(hdesc),
    _help(hhelp),
    _inputDesc(hinputDesc),
    _outputDesc(houtputDesc) {}

  inline void setFilename  (const QString     &filename   ) {_filename=filename;       }
  inline void setName      (const QString     &hname      ) {_name=hname;              }
  inline void setPath      (const QString     &hpath      ) {_path=hpath;              }
  inline void setDesc      (const QString     &hdesc      ) {_desc=hdesc;              }
  inline void setHelp      (const QString     &hhelp      ) {_help=hhelp;              }
  inline void setInputDesc (const QStringList &hinputDesc ) {_inputDesc  = hinputDesc; }
  inline void setOutputDesc(const QStringList &houtputDesc) {_outputDesc = houtputDesc;}

  inline const QString     filename  () {return _filename;  }
  inline const QString     name      () {return _name;      }
  inline const QString     path      () {return _path;      }
  inline const QString     desc      () {return _desc;      }
  inline const QString     help      () {return _help;      }
  inline const QStringList inputDesc () {return _inputDesc; }
  inline const QStringList outputDesc() {return _outputDesc;}
  
  inline virtual NodeInterface *createInstance(PbGraph *parent,unsigned int nbInputs,unsigned int nbOutputs) {
    return NodeManager::instance()->loadCustomNode(parent,this,nbInputs,nbOutputs);
  }

 private:
  QString _filename;
  QString _name;
  QString _path;
  QString _desc;
  QString _help;
  QStringList _inputDesc;
  QStringList _outputDesc;
};

// this class is used to remember the original node information for a custom node 
class OrigNodeHandle : public CustomNodeHandle {
 public:
 OrigNodeHandle(const QString &hname=QString(),
		const QString &hpath=QString(),
		const QString &hdesc=QString(),
		const QString &hhelp=QString(),
		const QStringList &hinputDesc=QStringList(),
		const QStringList &houtputDesc=QStringList()) 
   : CustomNodeHandle("",hname,hpath,hdesc,hhelp,hinputDesc,houtputDesc) {}
  inline virtual NodeInterface *createInstance(PbGraph *,unsigned int,unsigned int) {return NULL;}
};

#endif // NODE_MANAGER_H
