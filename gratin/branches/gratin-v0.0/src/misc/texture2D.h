// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H

#include "misc/glinclude.h"
#include "misc/textureFormat.h"

#include <string>
#include <assert.h>

template<typename T = float> 
  class Texture2D {
 public:

 Texture2D(const TextureFormat &tf=TextureFormat(),
           const TextureParams &tp=TextureParams(),
           T* map=NULL,
           int id=-1);

 Texture2D(const Texture2D &tex);

 ~Texture2D();

 inline GLuint        id    () const;
 inline TextureFormat format() const;
 inline TextureParams params() const;
 inline void          bind  () const;
 inline unsigned int  w     () const {return _w;}
 inline unsigned int  h     () const {return _h;}

 inline void          setData(const TextureFormat &tf,
			      const TextureParams &tp,
			      T* map);
 
 protected:
 GLuint        _id;
 TextureFormat _format;
 TextureParams _params;
 unsigned int  _w;
 unsigned int  _h;
};

template<typename T>
inline GLuint Texture2D<T>::id() const {
  return _id;
}

template<typename T>
inline TextureFormat Texture2D<T>::format() const {
  return _format;
}

template<typename T>
inline TextureParams Texture2D<T>::params() const {
  return _params;
}

template<typename T>
inline void Texture2D<T>::bind() const {

  glBindTexture(_format.target(),_id);
}

template<typename T>
Texture2D<T>::Texture2D(const Texture2D<T> &tex)
: _id(tex.id()),
  _format(tex.format()),
  _params(tex.params()),
  _w(_format.width()),
  _h(_format.height()) {

}

template<typename T>
Texture2D<T>::Texture2D(const TextureFormat &tf,const TextureParams &tp,T* map,int id)
: _id(id),
  _format(tf),
  _params(tp),
  _w(_format.width()),
  _h(_format.height()) {

  glEnable(GL_TEXTURE_2D);

  if(id<0 || glIsTexture(id)==GL_FALSE) {
    glGenTextures(1,&_id);
  } else {
    _id = id;
  }

  setData(tf,tp,map);
  
  }
  

template<typename T>
inline void Texture2D<T>::setData(const TextureFormat &tf,const TextureParams &tp,T* map) {
  _format = tf;
  _params = tp;
  _w = _format.width();
  _h = _format.height();

  glBindTexture(_format.target(),_id);
    
  glTexImage2D(_format.target(),
	       _format.level(),
	       _format.internalformat(),
	       _format.width(),
	       _format.height(),
	       _format.border(),
	       _format.format(),
	       _format.type(),
	       (const GLvoid *)map);
  
  float v[4] = {0.0f, 0.0f, 0.0f, 0.0f};
  glTexParameterfv(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_BORDER_COLOR, v);
  glTexParameteri(_format.target(),GL_TEXTURE_MIN_FILTER,_params.minfilter());
  glTexParameteri(_format.target(),GL_TEXTURE_MAG_FILTER,_params.maxfilter()); 
  glTexParameteri(_format.target(),GL_TEXTURE_WRAP_S,_params.wraps());
  glTexParameteri(_format.target(),GL_TEXTURE_WRAP_T,_params.wrapt());
  glTexParameteri(_format.target(),GL_TEXTURE_WRAP_R,_params.wrapr());

  if(_format.mipmapmode()==TextureFormat::MIPMAP_FBO_AUTOM) {
    glGenerateMipmap(_format.target());
  }

  glBindTexture(GL_TEXTURE_2D,0);  
}
template<typename T>
Texture2D<T>::~Texture2D() {
  glDeleteTextures(1,&_id);
}

typedef Texture2D<float>         FloatTexture2D;
typedef Texture2D<unsigned char> UbyteTexture2D;
typedef Texture2D<unsigned int>  UintTexture2D;

#endif // TEXTURE_2D_H