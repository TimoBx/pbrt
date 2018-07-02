// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2015-2016 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "multiImgLoader.h"
#include <QString>
#include <QStringList>
#include <iostream>

#include "misc/extinclude.h"
#include "misc/glutils.h"
#include "core/pbgraph.h"
#include "core/textureIO.h"
#include "multiImgLoaderWidget.h"

using namespace std;

MultiImgLoaderNode::MultiImgLoaderNode(PbGraph *parent,NodeHandle *handle)
: NodeTexture2D(parent,handle),
  _p(":/multiImgLoader.vert",":/multiImgLoader.frag"),
  _current(0) {

  _filenames = vector<QString>();
  _texs = vector<FloatTexture2D *>();
  _sizes = vector<Vector2i>();

  _filenames.push_back(QString(GRATIN_APPLI_DATA_PATH+"/imgs/default-tex.jpg"));
  _texs.push_back(Glutils::qimageToTexture(_filenames[0]));
  _sizes.push_back(Vector2i(_texs[0]->w(),_texs[0]->h()));

  _w = new MultiImgLoaderWidget(this);

  _p.addUniform("img");

}

MultiImgLoaderNode::~MultiImgLoaderNode() {
  for(unsigned int i=0;i<_texs.size();++i) 
    delete _texs[i];
  _texs.clear();
  _filenames.clear();
  _sizes.clear();
}

void MultiImgLoaderNode::apply() {
  if(_w->testImageChanged(_current)) {
    _w->currentImageChanged();
  }

  Glutils::setViewport(outputTex(0)->w(),outputTex(0)->h());

  _p.enable();
  _p.setUniformTexture("img",GL_TEXTURE_2D,currentTex()->id());
  drawOutputs(buffersOfOutputTex(0));
  _p.disable();
}

void MultiImgLoaderNode::loadImages(const vector<QString >&filenames,const vector<Vector2i> &sizes,int current) {
  makeCurrent();

  // test if at least one file is a texture 
  bool found = false;
  for(unsigned int i=0;i<filenames.size();++i) {
    TextureIO loader;

    if(filenames[i].isEmpty()) continue;

    FloatTexture2D *t = loader.load(filenames[i]);
    if(t) {
      found = true;
      delete t;
      break;
    }
  }

  if(!found)
    return;

  // clear all 
  for(unsigned int i=0;i<_texs.size();++i) {
    delete _texs[i];
  }
  _texs.clear();
  _filenames.clear();
  _sizes.clear();

  // load all textures 
  bool loadAll = ((MultiImgLoaderWidget *)widget())->loadAllImages();

  for(unsigned int i=0;i<filenames.size();++i) {
    TextureIO loader;
    QDir d(".");

    if(filenames[i].isEmpty()) continue;

    FloatTexture2D *t = loader.load(filenames[i]);
    
    if(!t) continue;

    _filenames.push_back(d.absoluteFilePath(filenames[i]));

    if(i<sizes.size())
      _sizes.push_back(sizes[i]);    
    else 
      _sizes.push_back(Vector2i(t->w(),t->h()));    
    
    if(!loadAll) {
      delete t;
      t = NULL;
    }
    _texs.push_back(t);
  }

  // modify current tex and sizes 
  if(current<0 || current>((int)_texs.size()-1)) 
    _current = 0;
  else
    _current = (unsigned int)current;

  if(!loadAll) {
    TextureIO loader;
    _texs[_current] = loader.load(_filenames[_current]);
  }

  _p.setUniformTexture("img",GL_TEXTURE_2D,currentTex()->id());
  
  if(sizes.empty()) 
    setOutputSize(Vector2f(currentTex()->w(),currentTex()->h()));
  else {
    Vector2i s = currentSize();
    setOutputSize(Vector2f((float)s[0],(float)s[1]));
  }
  
  ((MultiImgLoaderWidget *)widget())->setSliderVals(nbImages(),currentIndex());
  ((MultiImgLoaderWidget *)widget())->imgSizeChanged(Vector2i(currentTex()->w(),currentTex()->h()),currentSize());
}

bool MultiImgLoaderNode::save(SceneSaver  *saver) {
  bool ret = true;
  ret = ret && saver->saveUint("nbImages",_texs.size());
  ret = ret && saver->saveUint("currentImage",_current);

  for(unsigned int i=0;i<_texs.size();++i) {
    QString name = saver->absoluteToRelativePath(_filenames[i]);
    ret = ret && saver->saveString(Tags::attribId("filename",i),name);
    ret = ret && saver->saveVector2i(Tags::attribId("imgSize",i),_sizes[i]);
  }

  return ret;
}

bool MultiImgLoaderNode::load(SceneLoader *loader) {
  // clean all 
  for(unsigned int i=0;i<_texs.size();++i) 
    delete _texs[i];
  _texs.clear();
  _filenames.clear();
  _sizes.clear();
  _current = 0;

  bool ret = true;
  unsigned int nbTex = 0;
  
  ret = ret && loader->loadUint("nbImages",nbTex);
  ret = ret && loader->loadUint("currentImage",_current);

  vector<QString>  tmpNames;
  vector<Vector2i> tmpSizes;

  for(unsigned int i=0;i<nbTex;++i) {
    QString tmpName;
    Vector2i tmpSize;
    bool r1 = loader->loadString(Tags::attribId("filename",i),tmpName);
    bool r2 = loader->loadVector2i(Tags::attribId("imgSize",i),tmpSize);

    if(r1 && r2) {
      tmpNames.push_back(loader->relativeToAbsolutePath(tmpName));
      tmpSizes.push_back(tmpSize);
    } else {
      ret = false;
    }
  }

  if(tmpNames.empty()) {
    tmpNames.push_back(QString(GRATIN_APPLI_DATA_PATH+"/imgs/default-tex.jpg"));
    tmpSizes.push_back(Vector2i(512,512));
    _current = 0;
  } 
  
  if(_current>=tmpNames.size()) {
    _current = 0;
  }

  loadImages(tmpNames,tmpSizes,_current);

  return ret;
}

void MultiImgLoaderNode::setCurrentIndex(unsigned int index) {
  if(nbImages()==0) { 
    _current = 0;
    return;
  }
  
  unsigned int tmp = _current;
  Vector2i so = currentSize();
  _current = std::min(index,nbImages()-1);
  Vector2i sn = currentSize();

  bool loadAll = ((MultiImgLoaderWidget *)widget())->loadAllImages();
  if(!loadAll) {
    TextureIO loader;
    delete _texs[tmp];
    _texs[tmp] = NULL;
    _texs[_current] = loader.load(_filenames[_current]);
  }

  if(so!=sn) {
    setOutputSize(Vector2f((float)sn[0],(float)sn[1]));
  } 
}


void MultiImgLoaderNode::setImgSize(const Vector2i &size,bool applyToAll) {
  _sizes[_current] = size;
  setOutputSize(Vector2f((float)size[0],(float)size[1]));

  if(applyToAll) {
    for(unsigned int i=0;i<nbImages();++i) {
      _sizes[i] = size;
    } 
  }
}

void MultiImgLoaderNode::reload() {
  _p.reload();
}

void MultiImgLoaderNode::loadAllChanged() {
  bool loadAll = ((MultiImgLoaderWidget *)widget())->loadAllImages();

  for(unsigned int i=0;i<_texs.size();++i) {
    TextureIO loader;
    if(loadAll && !_texs[i]) {
      _texs[i] = loader.load(_filenames[i]);
    } else if(!loadAll) {
      if(i==_current && !_texs[i]) {
	_texs[i] = loader.load(_filenames[i]);
      } else if(i!=_current && _texs[i]) {
	delete _texs[i];
	_texs[i] = NULL;
      }
    }
  }
}
