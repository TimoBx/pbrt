// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef GLUTILS_H
#define GLUTILS_H

#include "glinclude.h"
#include "texture1D.h"
#include "texture2D.h"
#include "texture3D.h"
#include <Eigen/Dense>
#include <QImage>
#include <QGLWidget>
#include <QString>
#include <iostream>

using namespace Eigen;

class LIBRARY_EXPORT Glutils {
 public:
  inline static GLenum defFormat() {return GL_RGBA32F;}
  inline static GLenum defFilter()   {return GL_LINEAR;}

  inline static void setViewport(GLsizei w,GLsizei h) {glViewport(0,0,w,h);}

  inline static const Matrix4f ortho(float l,float r,float b,float t,float n=-1.0f,float f=1.0f) {
    Matrix4f o;
    const float rml = r-l;
    const float tmb = t-b;
    const float fmn = f-n;
    o << 2.0f/rml,0,0,-(r+l)/rml,0,2.0f/tmb,0,-(t+b)/tmb,0,0,-2.0f/fmn,-(f+n)/fmn,0,0,0,1;
    return o;
  }

  inline static const Matrix4f squareMode(unsigned int w,unsigned int h) {
    setViewport(w,h); return ortho(0,w-1,h-1,0,0.0,-1.0);
  }

  inline static const Matrix4f screenMode(unsigned int w,unsigned int h,const Vector2f &offset,float scale) {
    setViewport(w,h);

    const float w2 = scale*(w/2.0f);
    const float h2 = scale*(h/2.0f);

    return ortho(-w2+offset[0],w2+offset[0],-h2+offset[1],h2+offset[1],0.0,-1.0);
  }

  static inline FloatTexture2D *qimageToTexture(const QString &filename,bool mipmap=false) {
    QImage image(filename);
    if(image.isNull()) {
      return NULL;
    }

    QImage glimage = QGLWidget::convertToGLFormat(image);
    
    return mipmap ? 
      new FloatTexture2D(TextureFormat(GL_TEXTURE_2D,glimage.width(),glimage.height(),
				       defFormat(),GL_RGBA,GL_UNSIGNED_BYTE,0,TextureFormat::MIPMAP_FBO_AUTOM),
			 TextureParams(GL_LINEAR_MIPMAP_LINEAR,GL_LINEAR),(float *)glimage.bits()) :
      new FloatTexture2D(TextureFormat(GL_TEXTURE_2D,glimage.width(),glimage.height(),
				       defFormat(),GL_RGBA,GL_UNSIGNED_BYTE),
			 TextureParams(GL_LINEAR,GL_LINEAR),(float *)glimage.bits());
  }

  inline static bool glError(const QString &text=QString()) {
    std::cout << text.toStdString() << " --- ";
    GLenum e = glGetError();
    switch(e) {
    case GL_NO_ERROR: std::cout << "No error" << std::endl; return false; break;
    case GL_INVALID_ENUM: std::cout << "Error: invalid enum" << std::endl;break;
    case GL_INVALID_VALUE: std::cout << "Error: invalid value" << std::endl;break;
    case GL_INVALID_OPERATION: std::cout << "Error: invalid operation" << std::endl;break;
    case GL_INVALID_FRAMEBUFFER_OPERATION: std::cout << "Error: invalid framebuffer operation" << std::endl;break;
    case GL_OUT_OF_MEMORY: std::cout << "Error: out of memory" << std::endl;break;
    case GL_STACK_OVERFLOW: std::cout << "Error: stack overflow" << std::endl;break;
    case GL_STACK_UNDERFLOW: std::cout << "Error: stack underflow" << std::endl;break;
    default: std::cout << "Error: unknown" << std::endl;break;
    }

    return true;
  }
};

#endif // GLUTILS_H
