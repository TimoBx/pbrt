// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef GL_INCLUDE_H
#define GL_INCLUDE_H

#if defined(LIBRARY)
#  define LIBRARY_EXPORT Q_DECL_EXPORT
#else
#  define LIBRARY_EXPORT Q_DECL_IMPORT
#endif

#include <QObject>

#ifdef _MACX_ 
#include <GL/glew.h> 
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#elif defined(_WIN32_)
#include <GL/glew.h> 
#include <GL/gl.h>
#include <GL/glu.h>

#else // unix 
#include <GL/glew.h> 
#include <GL/gl.h>
#include <GL/glu.h>

#endif

#include <Eigen/Dense>

using namespace Eigen;

#ifndef EIGEN_VECTOR_U
#define EIGEN_VECTOR_U
typedef Matrix< unsigned int , 2 , 1> Vector2u;
typedef Matrix< unsigned int , 3 , 1> Vector3u;
typedef Matrix< unsigned int , 4 , 1> Vector4u;
#endif

#include<Eigen/StdVector>
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Vector2i)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Vector2f)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Vector2d)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Vector2u)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Vector3i)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Vector3f)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Vector3d)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Vector3u)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Vector4i)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Vector4f)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Vector4d)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Vector4u)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(VectorXi)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(VectorXf)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(VectorXd)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Matrix2f)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Matrix2d)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Matrix3f)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Matrix3d)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Matrix4f)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Matrix4d)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(MatrixXf)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(MatrixXd)

#endif // GL_INCLUDE_H
