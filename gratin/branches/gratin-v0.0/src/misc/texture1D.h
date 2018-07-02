// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef TEXTURE_1D_H
#define TEXTURE_1D_H

#include "misc/glinclude.h"
#include "misc/textureFormat.h"

#include <assert.h>
#include <string>


template<typename T = float> 
  class Texture1D {
 public:

 static const int LINE   = 0;
 static const int COLUMN = 1;

 Texture1D(const TextureFormat &tf=TextureFormat(),
	   const TextureParams &tp=TextureParams(),
	   T* map=NULL,
	   int id=-1);



 ~Texture1D();

 inline GLuint        id    () const;
 inline TextureFormat format() const;
 inline TextureParams params() const;
 inline void          bind  () const;
 inline unsigned int  w     () const {return _w;}

 inline void          setData(const TextureFormat &tf,
			      const TextureParams &tp,
			      T* map);


 protected:
 GLuint        _id;
 TextureFormat _format;
 TextureParams _params;
 unsigned int  _w;
};

template<typename T>
inline GLuint Texture1D<T>::id() const {
  return _id;
}

template<typename T>
inline TextureFormat Texture1D<T>::format() const {
  return _format;
}

template<typename T>
inline TextureParams Texture1D<T>::params() const {
  return _params;
}

template<typename T>
inline void Texture1D<T>::bind() const {

  glBindTexture(_format.target(),_id);

}

template<typename T>
Texture1D<T>::Texture1D(const TextureFormat &tf,const TextureParams &tp,T* map,int id)
: _id(id),
  _format(tf),
  _params(tp),
  _w(_format.width()) {

  if(id<0 || glIsTexture(id)==GL_FALSE) {
    glGenTextures(1,&_id);
  } else {
    _id = id;
  }

  setData(tf,tp,map);

  }

template<typename T>
inline void Texture1D<T>::setData(const TextureFormat &tf,const TextureParams &tp,T* map) {
  _format = tf;
  _params = tp;
  _w      = _format.width();

  glBindTexture(_format.target(),_id);
    
  glTexImage1D(_format.target(),
	       _format.level(),
	       _format.internalformat(),
	       _format.width(),
	       _format.border(),
	       _format.format(),
	       _format.type(),
	       (const GLvoid *)map);
  
  if(_format.mipmapmode()==TextureFormat::MIPMAP_FBO_AUTOM) {
    assert(map!=NULL && map!=0);
    glGenerateMipmapEXT(_format.target()); 
  }

  float v[4] = {0.0f, 0.0f, 0.0f, 0.0f};
  glTexParameterfv(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_BORDER_COLOR, v);
  glTexParameteri(_format.target(),GL_TEXTURE_MIN_FILTER,_params.minfilter());
  glTexParameteri(_format.target(),GL_TEXTURE_MAG_FILTER,_params.maxfilter()); 
  glTexParameteri(_format.target(),GL_TEXTURE_WRAP_S,_params.wraps());
  glTexParameteri(_format.target(),GL_TEXTURE_WRAP_T,_params.wrapt());

  glBindTexture(GL_TEXTURE_1D,0);
}


template<typename T>
Texture1D<T>::~Texture1D() {	
  glDeleteTextures(1,&_id);
}

typedef Texture1D<float>         FloatTexture1D;
typedef Texture1D<unsigned char> UbyteTexture1D;
typedef Texture1D<unsigned int>  UintTexture1D;


#endif // TEXTURE_1D_H
