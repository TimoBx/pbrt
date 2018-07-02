// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef VERTEXBUFFER_OBJECT_H
#define VERTEXBUFFER_OBJECT_H

#include "glinclude.h"
#include <assert.h>
#include <vector>
#include <iostream>

struct BufferData {
  GLuint      id;
  GLsizeiptr  buffersize;
  GLint       attribsize;
  void       *data; 
  GLuint      index;
  GLsizei     stride;
};


struct IndexData {
  GLuint        id;
  GLsizeiptr    buffersize;
  GLvoid       *data;
  GLenum        mode; 
  GLsizei       count;
};

class LIBRARY_EXPORT VertexbufferObject {

 public:
  VertexbufferObject(GLenum usage=GL_STATIC_DRAW)
    : _target(GL_ARRAY_BUFFER),
      _type(GL_FLOAT),
      _usage(usage),
      _indexTarget(GL_ELEMENT_ARRAY_BUFFER),
      _indexType(GL_UNSIGNED_INT),
      _indices(NULL) {}

  ~VertexbufferObject() {clear();}

  inline void setAttrib (GLsizeiptr buffersize,const void *data,GLuint index,GLint attribsize=4,GLsizei stride=0);
  inline void setIndices(GLsizeiptr buffersize,const void *data,GLsizei count,GLenum mode=GL_TRIANGLES);

  inline void removeIndices  ();
  inline void removeAttrib   (GLuint index);
  inline void clear          ();

  inline void bind();
  inline void enable();
  inline void disable();
  inline void draw();
  inline void draw(GLsizei count);
  inline void enableDrawAndDisable();
  inline void unbind();

 private:
  GLenum _target;
  GLenum _type;
  GLenum _usage;
  GLenum _indexTarget;
  GLenum _indexType;

  IndexData  *_indices;

  std::vector<BufferData *> _userAttribs;

  inline void removeBufferData(BufferData *&bd);
  inline void removeIndexData (IndexData  *&id);
};

inline void VertexbufferObject::bind() {
  for(unsigned int i=0;i<_userAttribs.size();++i) {
    glBindBuffer(_target,_userAttribs[i]->id);
    glVertexAttribPointer(_userAttribs[i]->index,_userAttribs[i]->attribsize,_type,GL_FALSE,_userAttribs[i]->stride,0);
  }

  if(_indices!=NULL) {
    glBindBuffer(_indexTarget,_indices->id);
  }
}

inline void VertexbufferObject::unbind() {
  glBindBuffer(_indexTarget, 0);
  glBindBuffer(_target, 0);
}

inline void VertexbufferObject::enable() {
  for(unsigned int i=0;i<_userAttribs.size();++i) {
    glEnableVertexAttribArray(_userAttribs[i]->index);
  }
}

inline void VertexbufferObject::disable() {
  for(unsigned int i=0;i<_userAttribs.size();++i) {
    glDisableVertexAttribArray(_userAttribs[i]->index);
  }
}

inline void VertexbufferObject::draw() {
  assert(_indices!=NULL);

  glDrawElements(_indices->mode,_indices->count,_indexType,0);
}

inline void VertexbufferObject::draw(GLsizei count) {
  assert(_indices!=NULL);

  glDrawElements(_indices->mode,count,_indexType,0);
}

inline void VertexbufferObject::enableDrawAndDisable() {
  enable();
  draw();
  disable();
}

inline void VertexbufferObject::setIndices(GLsizeiptr buffersize,const void *data,GLsizei count,GLenum mode) {
  // check if there is nothing... prevent opengl errors
  if(buffersize==0) return;

  assert(data!=NULL);

  if(_indices==NULL) {
    _indices = new IndexData;
    glGenBuffers(1,&(_indices->id));
  }

  _indices->buffersize = buffersize;
  _indices->data       = (void *)data;
  _indices->count      = count;
  _indices->mode       = mode;

  glBindBuffer(_indexTarget,_indices->id);
  glBufferData(_indexTarget,_indices->buffersize,_indices->data,_usage);
  unbind();
}

inline void VertexbufferObject::setAttrib(GLsizeiptr buffersize,const void *data,GLuint index,GLint attribsize,GLsizei stride) {

  // check if there is nothing... prevent opengl errors
  if(buffersize==0) return;

  assert(data!=NULL);

  BufferData *attrib = NULL;
  for(unsigned int i=0;i<_userAttribs.size();++i) {
    if(_userAttribs[i]->index==index) {
      attrib = _userAttribs[i];
      break;
    }
  }

  if(attrib==NULL) {
    attrib = new BufferData;
    glGenBuffers(1,&(attrib->id));
    attrib->index = index;
    _userAttribs.push_back(attrib);
  }

  attrib->buffersize = buffersize;
  attrib->attribsize = attribsize;
  attrib->data       = (void *)data;
  attrib->stride     = stride;

  glBindBuffer(_target,attrib->id);
  glBufferData(_target,attrib->buffersize,attrib->data,_usage);
  unbind();
  //glVertexAttribPointer(attrib->index,attrib->attribsize,_type,GL_FALSE,attrib->stride,0);
}

inline void VertexbufferObject::removeIndices() {
  if(_indices!=NULL)
    removeIndexData(_indices);
}

inline void VertexbufferObject::removeAttrib(GLuint index) {
  for(unsigned int i=0;i<_userAttribs.size();++i) {
    if(_userAttribs[i]->index==index) {
      removeBufferData(_userAttribs[i]);
      return;
    }
  }
}

inline void VertexbufferObject::clear() {
  if(_indices!=NULL)
    removeIndexData(_indices);    
  
  for(unsigned int i=0;i<_userAttribs.size();++i) {
    removeBufferData(_userAttribs[i]);
  }
  
  _userAttribs.clear();
}

inline void VertexbufferObject::removeBufferData(BufferData *&bd) {
  glDeleteBuffers(1,&(bd->id));
  delete bd;
  bd = NULL;
}

inline void VertexbufferObject::removeIndexData(IndexData *&id) {
  glDeleteBuffers(1,&(id->id));
  delete id;
  id = NULL;
}

#endif // VERTEXBUFFER_OBJECT_H
