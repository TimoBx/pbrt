// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "misc/gpuShader.h"
#include "misc/glutils.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>

#include <QtGui>
#include <QTextStream>
#include <QString>
#include <QFile>

using namespace std;
      
GPUShader::GPUShader(SHADER_TYPE type,const string &filename,bool printLog,bool fromFile)
  : _filename(filename),
    _log(string()),
    _type(type),
    _shaderId(0),
    _printLog(printLog),
    _fromFile(fromFile) {
  _created = createShader();

  loadAndCompile();
}

GPUShader::~GPUShader() {
  if(_created){
    glDeleteShader(_shaderId);
  }
}

bool GPUShader::load(RList replace) {

  QString res;
  if(_fromFile) {
    QFile f(QString(_filename.c_str()));
    
    if (!f.open(QFile::ReadOnly)) {
      std::cerr << "failed to load shader file " << _filename << "\n";
      return false;
    }
    
    QTextStream stream(&f);
    res = stream.readAll();
    f.close();
  } else {
    res = QString(_filename.c_str());
  }

  for(int i=0;i<replace.size();++i) {
    res = res.replace(replace[i].first,replace[i].second);
  }

  string tmp = res.toStdString();
  int l = (int)tmp.length();

  const char *s = tmp.c_str();

  glShaderSource(_shaderId,1,&s,&l);
  return true;
}
  

bool GPUShader::compile() { 
  glCompileShader(_shaderId);
  
  if(_printLog)
    printInfoLog();

  return true;
}

void GPUShader::printInfoLog() {
  int   infologLength = 0;
  int   charsWritten  = 0;
  char* infolog;
    
  glGetObjectParameterivARB(_shaderId,GL_OBJECT_INFO_LOG_LENGTH_ARB,&infologLength);
    
  if(infologLength>0) {
    infolog = (char*)malloc(infologLength);
    glGetInfoLogARB(_shaderId,infologLength,&charsWritten,infolog);
    _log = string(infolog);
    if(infolog[0]!='\0') {
      printf("\n\n\n===============================\nInfoLog ---> %s\n",_filename.c_str());
      printf("%s",infolog);
    }
    free(infolog);
  }
}

bool GPUShader::loadAndCompile(RList replace) {
  return _created && load(replace) && compile();
}

bool GPUShader::createShader() {
  switch(_type) {

  case VERT:
    if(GLEW_ARB_vertex_shader)
      _shaderId = glCreateShader(GL_VERTEX_SHADER);
    else { 
      cout << "Warning : vertex shader not supported !" << endl; 
      return false;
    }
    break;
      
  case FRAG:
    if(GLEW_ARB_fragment_shader){
      _shaderId = glCreateShader(GL_FRAGMENT_SHADER);
    }
    else { 
      cout << "Warning : fragment shader not supported !" << endl; 
      return false;
    }
    break;
      
  case GEOM:
#ifdef GL_EXT_geometry_shader4
    if(GL_EXT_geometry_shader4){
      _shaderId = glCreateShader(GL_GEOMETRY_SHADER_EXT);
    }
    else { 
      cout << "Warning : geometry shader not supported !" << endl;  
      return false;
    }
#else 
    cout << "Warning : geometry shader not supported !" << endl;  
    return false;
#endif
    break;
      
  default:
    cout << "Warning : unknown shader type !" << endl;
    return false;
    break;
  }
    
  if(_shaderId==0) {
    cout << "Warning : shader " << _filename << " is not created !" << endl;
    return false;
  }
      
  return true;
} 
