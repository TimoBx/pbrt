// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "misc/gpuProgram.h"
#include "misc/glutils.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

using namespace std;
      

GPUProgram::GPUProgram(GPUShader* vs,GPUShader* fs,GPUShader *gs,
                       int inputGeometry,int outputGeometry,int outVertices)
  : _vs(vs),
    _fs(fs),
    _gs(gs),
    _inputGeometry(inputGeometry),
    _outputGeometry(outputGeometry),
    _outVertices(outVertices) {

  _programId = _glf->glCreateProgram();
  setGeometryParameters(_inputGeometry,_outputGeometry,_outVertices);
  attachAndLink();
    }

GPUProgram::GPUProgram(const string &vsFile,
                       const string &fsFile,
                       const string &gsFile,
		       bool fromFile,
                       int inputGeometry,
                       int outputGeometry,
                       int outVertices)
  :_inputGeometry(inputGeometry),
   _outputGeometry(outputGeometry),
   _outVertices(outVertices) {

  _vs = _fs = _gs = NULL;

  if(vsFile!="")
    _vs = new GPUShader(VERT,vsFile,true,fromFile);

  if(fsFile!="")
    _fs = new GPUShader(FRAG,fsFile,true,fromFile);

  if(gsFile!="")
    _gs = new GPUShader(GEOM,gsFile,true,fromFile);

  _programId = _glf->glCreateProgram();
  setGeometryParameters(_inputGeometry,_outputGeometry,_outVertices);
  attachAndLink();
   }

      
GPUProgram::~GPUProgram() {
      
  detach();
    
  if(_vs!=NULL) {
    delete _vs;
  }

  if(_fs!=NULL) {
    delete _fs;
  }

  if(_gs!=NULL) {
    delete _gs;
  }
    
  _glf->glDeleteProgram(_programId);
}

void GPUProgram::setGeometryParameters(int inputGeometry,int outputGeometry,int outVertices) {

  if(_gs!=NULL && _gs->id()!=0) {
    _glf->glProgramParameteri(_programId,GL_GEOMETRY_INPUT_TYPE,inputGeometry);
    _glf->glProgramParameteri(_programId,GL_GEOMETRY_OUTPUT_TYPE,outputGeometry);
    _glf->glProgramParameteri(_programId,GL_GEOMETRY_VERTICES_OUT,outVertices);
  }

}

void GPUProgram::attach() {
  if(_vs!=NULL) {
    _glf->glAttachShader(_programId,_vs->id()); 
  }

  if(_fs!=NULL) {
    _glf->glAttachShader(_programId,_fs->id());
  }

  if(_gs!=NULL) {
    _glf->glAttachShader(_programId,_gs->id());       
  }
}

void GPUProgram::detach() {
        
  if(_vs!=NULL) {
    _glf->glDetachShader(_programId,_vs->id()); 
  }

  if(_fs!=NULL) {
    _glf->glDetachShader(_programId,_fs->id());
  }

  if(_gs!=NULL) {
    _glf->glDetachShader(_programId,_gs->id());  
  }
}

bool GPUProgram::link() {
  //int linked = 1;

  _glf->glLinkProgram(_programId);
      
  //_glf->glGetObjectParameteriv(_programId,GL_OBJECT_LINK_STATUS,&linked);
       
  //if(linked)
    return true;
    
    //return false;
}

bool GPUProgram::attachAndLink() {
  attach();
  return link();
}

void GPUProgram::reload(GPUShader::RList replace) {
      
  detach();

  bool allOk = true;
  if(_vs!=NULL) {
    allOk = allOk && _vs->loadAndCompile(replace);      
  }

  if(_fs!=NULL) {
    allOk = allOk && _fs->loadAndCompile(replace);
  }

  if(_gs!=NULL) {
    allOk = allOk && _gs->loadAndCompile(replace);
  }
  
  if(!allOk){
    std::cout << "reload fail, maybe missing file" << std::endl;
  }
  setGeometryParameters(_inputGeometry,_outputGeometry,_outVertices);
  attachAndLink();

  // reload uniforms 
  for(map<string,GLint>::iterator i=_uniformLocations.begin();i!=_uniformLocations.end();i++) {
    _uniformLocations[(*i).first] = _glf->glGetUniformLocation(_programId,(*i).first.c_str());
  }

  // reload attributes
  for(map<string,GLint>::iterator i=_attributeLocations.begin();i!=_attributeLocations.end();i++) {
    _uniformLocations[(*i).first] = _glf->glGetAttribLocation(_programId,(*i).first.c_str());
  }

  // free textures 
  _textures.clear();
}


void GPUProgram::addUniform(const string &uniformName) {
  GLint location = _glf->glGetUniformLocation(_programId,uniformName.c_str());  

  _uniformLocations[uniformName] = location;
}

void GPUProgram::addAttribute(const string &attributeName) {
  GLint location = _glf->glGetAttribLocation(_programId,attributeName.c_str());  
    
  _attributeLocations[attributeName] = location;
}

bool GPUProgram::haveShaderOfType(SHADER_TYPE type) {
    
  if(type==VERT)
    return _vs!=NULL;
    
  if(type==FRAG)
    return _fs!=NULL;
    
  if(type==GEOM)
    return _gs!=NULL;
    
  cout << "Warning : unknown type !" << endl;
  
  return false;
}

string GPUProgram::filename(SHADER_TYPE type) {
    
  if(type==VERT && _vs!=NULL)
    return _vs->filename();
    
  if(type==FRAG && _fs!=NULL)
    return _fs->filename();
    
  if(type==GEOM && _gs!=NULL)
    return _gs->filename();

  cout << "Warning : unknown type !" << endl;
    
  return "";
}

string GPUProgram::log(SHADER_TYPE type) {
    
  if(type==VERT && _vs!=NULL)
    return _vs->log();
    
  if(type==FRAG && _fs!=NULL)
    return _fs->log();
    
  if(type==GEOM && _gs!=NULL)
    return _gs->log();

  cout << "Warning : unknown type !" << endl;
    
  return "";
}


#ifdef SAFEUNILOCATION
#error SAFEUNILOCATION already defined. This will not work!
#endif
#define SAFEUNILOCATION					\
  GLint uniLoc = _glf->getUniformLocation(uniformName);	\
  if(uniLoc<0)						\
    {									\
      fprintf(stderr,"%s: Trying to set undefined uniform '%s' in '%s'\7\n",__FUNCTION__,uniformName.c_str(),_fs->filename().c_str()); \
    }

