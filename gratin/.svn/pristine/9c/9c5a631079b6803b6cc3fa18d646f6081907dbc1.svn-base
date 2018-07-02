// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef TEXT_DRAWER
#define TEXT_DRAWER

#include "misc/glinclude.h"
#include "misc/glutils.h"
#include "misc/gpuProgram.h"
#include "misc/texture2D.h"
#include <vector>

class LIBRARY_EXPORT TextDrawer {
 public:
  TextDrawer();
  ~TextDrawer();

  inline void draw(const QString  &text,
		   const Vector2f &pos,
		   const Vector2f &size,
		   const Matrix4f &mvp=Matrix4f::Identity(),
		   const Vector4f &tcolor=Vector4f(0,0,0,1),
		   const Vector4f &bcolor=Vector4f(0,0,0,0),
		   bool verticalMode=false);
		  
		   
 private:
  static void initGPUData();
  static void clearGPUData();

  static GPUProgram     *_ptext;
  static FloatTexture2D *_tfont;
  static int             _nbInstances;
  static Vector2f        _cellSize;
  static const int       _nb = 16;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

inline void TextDrawer::draw(const QString  &text,
			     const Vector2f &pos,
			     const Vector2f &size,
			     const Matrix4f &mvp,
			     const Vector4f &tcolor,
			     const Vector4f &bcolor,
			     bool) {
  if(text.isEmpty()) return;

  unsigned int s = (unsigned int)text.size();

  std::vector<Vector2f> vertices(6*s);
  std::vector<Vector2f> uvcoords(6*s);
  unsigned int j=0;

  /* if(verticalMode) {   */
  /*   float stepy = size[1]/(float)s; */

  /*   for(unsigned int i=0;i<s;++i) { */
  /*     const Vector2f p = pos+Vector2f(0,i*stepy); */
  /*     const char character = text[i].toAscii(); */
  /*     const float uvx = (character%16)/16.0f; */
  /*     const float uvy = (character/16)/16.0f; */
      
  /*     vertices[j  ] = p; */
  /*     vertices[j+1] = p+Vector2f(0,stepy); */
  /*     vertices[j+2] = p+Vector2f(-size[0],stepy); */
  /*     vertices[j+3] = vertices[j+2]; */
  /*     vertices[j+4] = p+Vector2f(-size[0],0); */
  /*     vertices[j+5] = p;  */
      
  /*     uvcoords[j  ] = Vector2f(uvx,1.0f-(uvy+1.0f/16.0f)); */
  /*     uvcoords[j+1] = Vector2f(uvx+1.0f/16.0f,1.0f-(uvy+1.0f/16.0f)); */
  /*     uvcoords[j+2] = Vector2f(uvx+1.0f/16.0f,1.0f-uvy); */
  /*     uvcoords[j+3] = uvcoords[j+2]; */
  /*     uvcoords[j+4] = Vector2f(uvx,1.0f-uvy); */
  /*     uvcoords[j+5] = uvcoords[j]; */
      
  /*     j+=6; */
  /*   } */
  /* } else { */
    float stepx = size[0]/(float)s;
    
    
    for(unsigned int i=0;i<s;++i) {
      const Vector2f p = pos+Vector2f(i*stepx,0);
      const char character = text[i].toAscii();
      const float uvx = (character%16)/16.0f;
      const float uvy = (character/16)/16.0f;
      
      vertices[j  ] = p;
      vertices[j+1] = p+Vector2f(stepx,0);
      vertices[j+2] = p+Vector2f(stepx,size[1]);
      vertices[j+3] = vertices[j+2];
      vertices[j+4] = p+Vector2f(0,size[1]);
      vertices[j+5] = p; 
      
      uvcoords[j  ] = Vector2f(uvx,1.0f-(uvy+1.0f/16.0f));
      uvcoords[j+1] = Vector2f(uvx+1.0f/16.0f,1.0f-(uvy+1.0f/16.0f));
      uvcoords[j+2] = Vector2f(uvx+1.0f/16.0f,1.0f-uvy);
      uvcoords[j+3] = uvcoords[j+2];
      uvcoords[j+4] = Vector2f(uvx,1.0f-uvy);
      uvcoords[j+5] = uvcoords[j];
      
      j+=6;
    }
    // }

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  
  glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,vertices[0].data());
  glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,uvcoords[0].data());

  _ptext->enable();
  _ptext->setUniformMatrix4fv("mvp",(GLfloat *)mvp.data());
  _ptext->setUniform4fv("tcolor",(GLfloat *)tcolor.data());
  _ptext->setUniform4fv("bcolor",(GLfloat *)bcolor.data());
  _ptext->setUniformTexture("font",GL_TEXTURE_2D,_tfont->id()); 

  glDrawArrays(GL_TRIANGLES,0,6*s); 
  
  _ptext->disable();
  
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);
}

#endif // TEXT_DRAWER
