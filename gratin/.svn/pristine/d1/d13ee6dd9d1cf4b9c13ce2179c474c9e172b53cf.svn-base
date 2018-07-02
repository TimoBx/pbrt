// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef CURVE
#define CURVE

#include <assert.h>
#include <Eigen/Dense>
#include <vector>
#include <QString>
#include <QStringList>

#include "core/sceneIOTags.h"

using namespace Eigen;


// Generic curve interpolator 
class CurveInterpolator : public IOData {
 public:
  CurveInterpolator(const Vector2f &pt=Vector2f(0,0));
  CurveInterpolator(CurveInterpolator *curve);
  virtual ~CurveInterpolator() {_points.clear();_tangents.clear();}

  // evaluate
  virtual float evalAt(float x) const = 0;

  // keyframing
  virtual void addKeyframe(const Vector2f &pt);
  virtual void setKeyframe(const Vector2f &pt,unsigned int i=0);
  virtual void delKeyframe(unsigned int i);
  virtual void setTangent(const Vector2f &pt,unsigned int i=0);

  // return points and lines that describe the curve 
  virtual const std::vector<Vector2f> samplePoints(float x1,float x2,unsigned int nb=100) const;
  virtual const std::vector<Vector2f> sampleLines (float x1,float x2,unsigned int nb=100) const;

  // accessors 
  inline unsigned int nbPoints()                   const {return _points.size();}
  inline unsigned int nbTangents()                 const {return _tangents.size();}
  inline const std::vector<Vector2f> &points  ()   const {return _points;}
  inline const std::vector<Vector2f> &tangents()   const {return _tangents;}
  inline const Vector2f &point  (unsigned int i=0) const { assert(i<nbPoints());   return _points[i];}
  inline const Vector2f &tangent(unsigned int i=0) const { assert(i<nbTangents()); return _tangents[i];}

  inline virtual bool useTangents() const {return false;}

  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

 protected:
  std::vector<Vector2f> _points;
  std::vector<Vector2f> _tangents;
  
 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};


// Linear interpolation
class LinearInterpolator : public CurveInterpolator {
 public: 
 LinearInterpolator(const Vector2f    &pt)    : CurveInterpolator(pt   ) {}
 LinearInterpolator(CurveInterpolator *curve) : CurveInterpolator(curve) {}

  inline float evalAt(float x) const {
    assert(!_points.empty());
    for(unsigned int i=0;i<nbPoints()-1;++i) {
      if(_points[i+1][0]>=x) {
	return _points[i][1]+(_points[i+1][1]-_points[i][1])*
	  ((x-_points[i][0])/(_points[i+1][0]-_points[i][0]));
      }
    }
    return _points[0][1];
  }

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};





// Step interpolation
class StepInterpolator : public CurveInterpolator {
 public: 
 StepInterpolator(const Vector2f    &pt   ) : CurveInterpolator(pt   ) {}
 StepInterpolator(CurveInterpolator *curve) : CurveInterpolator(curve) {}

  inline float evalAt(float x) const {
    assert(!_points.empty());
    for(unsigned int i=0;i<nbPoints()-1;++i) {
      if(_points[i+1][0]>=x) {
	return _points[i][1];
      }
    }
    return _points[0][1];
  }

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};


// Shepard interpolation
class ShepardInterpolator : public CurveInterpolator {
 public: 
 ShepardInterpolator(const Vector2f    &pt)    : CurveInterpolator(pt   ),_p(2.0),_eps(1.0e-10) {}
 ShepardInterpolator(CurveInterpolator *curve) : CurveInterpolator(curve),_p(2.0),_eps(1.0e-10) {}

  inline float evalAt(float x) const {
    assert(!_points.empty());
    
    double r = 0.0;
    for(unsigned int i=0;i<nbPoints();++i) {
      double d = 0.0;
      for(unsigned int j=0;j<nbPoints();++j) {
	d +=  w(x,_points[j][0]);
      } 
      r += (w(x,_points[i][0])*_points[i][1])/std::max(d,_eps);
    }
    
    return (float)r;
  }

 private:
  inline double w(float x,float xi) const {return pow(std::max((double)fabs(xi-x),_eps),-_p);}
  const double _p;
  const double _eps;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};


// Spline interpolation
class SplineInterpolator : public CurveInterpolator {
 public: 
 SplineInterpolator(const Vector2f    &pt)    : CurveInterpolator(pt   ) {nbPointsChanged(); computeSolution();}
 SplineInterpolator(CurveInterpolator *curve) : CurveInterpolator(curve) {nbPointsChanged(); computeSolution();}
  
  virtual void addKeyframe(const Vector2f &pt);
  virtual void setKeyframe(const Vector2f &pt,unsigned int i);
  virtual void delKeyframe(unsigned int i);

  virtual bool load(SceneLoader *loader);

  inline float evalAt(float x) const {
    assert(!_points.empty());
    
    if(nbPoints()==1) {
      // 1 point: return the current value
      return _points[0][1];
    }
    
    if(nbPoints()==2) {
      // 2 points: return the linear interpolation
      return _points[0][1]+(_points[1][1]-_points[0][1])*
	((x-_points[0][0])/(_points[1][0]-_points[0][0]));
    }

    // 3 or more points: cubic spline interpolation
    for(unsigned int i=0;i<nbPoints()-1;++i) {
      if(_points[i+1][0]>=x) {
	const unsigned int index = i*4;
	const float x2 = x*x;
	const float x3 = x2*x;
	return _x[index] + _x[index+1]*x + _x[index+2]*x2 + _x[index+3]*x3; 
      }
    }

    // default
    return _points[0][1];
  }

 private:
  void computeSolution();
  void nbPointsChanged();
  
  MatrixXf _A;
  VectorXf _b;
  VectorXf _x;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};


// Hermite interpolation
class HermiteInterpolator : public CurveInterpolator {
 public: 
 HermiteInterpolator(const Vector2f    &pt)    : CurveInterpolator(pt   ) {initTangents();}
 HermiteInterpolator(CurveInterpolator *curve) : CurveInterpolator(curve) {initTangents();}
  virtual void addKeyframe(const Vector2f &pt);
  virtual void delKeyframe(unsigned int i);
  virtual void setTangent(const Vector2f &pt,unsigned int i=0);
  inline virtual bool useTangents() const {return true;}

  inline float evalAt(float x) const {
    assert(!_points.empty());
    for(unsigned int i=0;i<nbPoints()-1;++i) {
      if(_points[i+1][0]>=x) {
	const float dx    = _points[i+1][0]-_points[i][0];
	const float t     = (x-_points[i][0])/dx;
	const float t2    = t*t;
	const float t3    = t2*t;
	const float h00   = 2.0f*t3-3.0f*t2+1.0f;
	const float h10   = t3-2.0f*t2+t;
	const float h01   = -2.0f*t3+3.0f*t2;
	const float h11   = t3-t2;
	const float tang1 = _tangents[i  ][1]>0 ? tan(acos(_tangents[i  ][0])) : -tan(acos(_tangents[i  ][0]));
	const float tang2 = _tangents[i+1][1]>0 ? tan(acos(_tangents[i+1][0])) : -tan(acos(_tangents[i+1][0]));
	return h00*_points[i][1]+h10*tang1*dx+h01*_points[i+1][1]+h11*tang2*dx;
      }
    }
    return _points[0][1];
  }

 private:
  void initTangents();

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};






// How the curve behaves before and after the first and last control points
class CurveBehavior {
 public:
  enum {LEFT,RIGHT};
  CurveBehavior(unsigned int side,CurveInterpolator *interp) : _side(side),_interp(interp) {}
  virtual ~CurveBehavior() {}

  virtual float evalAt(float x) const = 0;

  // return points and lines that describe the curve
  virtual const std::vector<Vector2f> samplePoints(float x1,float x2,unsigned int nb=100) const;
  virtual const std::vector<Vector2f> sampleLines (float x1,float x2,unsigned int nb=100) const;

  inline void setInterpolator(CurveInterpolator *interp) {_interp = interp;}

 protected:
  unsigned int       _side;
  CurveInterpolator *_interp;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};




// constant curve before and after the control points 
class ConstantBehavior : public CurveBehavior {
 public:
 ConstantBehavior(unsigned int side,CurveInterpolator *interp) : CurveBehavior(side,interp) {}

 inline float evalAt(float) const { 
   switch(_side) {
   case RIGHT: return _interp->point(_interp->nbPoints()-1)[1];break;
   case LEFT:  return _interp->point(0)[1];break;
   default:    return _interp->point(0)[1];break;
   } 
 }

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};





// constant curve before and after the control points 
class LinearBehavior : public CurveBehavior {
 public:
 LinearBehavior(unsigned int side,CurveInterpolator *interp) : CurveBehavior(side,interp) {}

 inline float evalAt(float x) const { 
   if(_interp->nbPoints()<2) return _interp->point(0)[1];
   
   Vector2f p1,p2;

   switch(_side) {
   case RIGHT: p1 = _interp->point(_interp->nbPoints()-2); p2 = _interp->point(_interp->nbPoints()-1);break;
   case LEFT:  p1 = _interp->point(0); p2 = _interp->point(1);break;
   default:    return _interp->point(0)[1];break;
   } 

   return ((p2[1]-p1[1])/(p2[0]-p1[0]))*(x-p1[0])+p1[1];
 }

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};




// repeat curve before and after the control points 
class RepeatBehavior : public CurveBehavior {
 public:
 RepeatBehavior(unsigned int side,CurveInterpolator *interp) : CurveBehavior(side,interp) {}

 inline float evalAt(float x) const { 
   if(_interp->nbPoints()<2) return _interp->point(0)[1];
   const Vector2f p1 = _interp->point(0);
   const Vector2f p2 = _interp->point(_interp->nbPoints()-1);
   const float    xr = p2[0]-p1[0];

   switch(_side) {
   case RIGHT: return _interp->evalAt(fmod(x-p1[0],xr)+p1[0]);
   case LEFT:  return _interp->evalAt(fmod(x-p1[0],xr)+xr+p1[0]);
   default:    return _interp->point(0)[1];break;
   }
 }

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};





// repeat and offset curve before and after the control points 
class RepeatOffsetBehavior : public CurveBehavior {
 public:
 RepeatOffsetBehavior(unsigned int side,CurveInterpolator *interp) : CurveBehavior(side,interp) {}

 inline float evalAt(float x) const { 
   if(_interp->nbPoints()<2) return _interp->point(0)[1];
   const Vector2f p1 = _interp->point(0);
   const Vector2f p2 = _interp->point(_interp->nbPoints()-1);
   const float    xr = p2[0]-p1[0]; 
   const float    yr = p2[1]-p1[1]; 
   const float    c  = floor((x-p1[0])/xr);

   switch(_side) {
   case RIGHT: return _interp->evalAt(fmod(x-p1[0],xr)+p1[0])+c*yr;
   case LEFT:  return _interp->evalAt(fmod(x-p1[0],xr)+xr+p1[0])+c*yr;
   default:    return _interp->point(0)[1];break;
   }
 }

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};





// repeat and mirror curve before and after the control points 
class RepeatMirrorBehavior : public CurveBehavior {
 public:
 RepeatMirrorBehavior(unsigned int side,CurveInterpolator *interp) : CurveBehavior(side,interp) {}

 inline float evalAt(float x) const { 
   if(_interp->nbPoints()<2) return _interp->point(0)[1];
   const Vector2f p1 = _interp->point(0);
   const Vector2f p2 = _interp->point(_interp->nbPoints()-1);
   const float    xr = p2[0]-p1[0]; 
   const float    nx = _side==RIGHT ? fmod(x-p1[0],xr)+p1[0] : fmod(x-p1[0],xr)+xr+p1[0];
   const float    c  = floor((x-p1[0])/xr);
   return _interp->evalAt((int)c%2 ? p2[0]-(nx-p1[0]) : nx);
 }

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};






// Curve... 
class Curve : public IOData {
 public:
  enum {LINEAR_INTERP=0,STEP_INTERP=1,SHEPARD_INTERP=2,SPLINE_INTERP=3,HERMITE_INTERP=4};
  enum {CONSTANT_BEHAVIOR=0,LINEAR_BEHAVIOR=1,REPEAT_BEHAVIOR=2,REPEAT_OFFSET_BEHAVIOR=3,REPEAT_MIRROR_BEHAVIOR=4};

  Curve(const Vector2f &pt,int interpolation=0,int leftBehavior=0,int rightBehavior=0);
  virtual ~Curve();

  // evaluation 
  inline float evalAt(float x) const {
    if(x<point(0)[0])            return _leftBehavior->evalAt(x);
    if(x>point(nbPoints()-1)[0]) return _rightBehavior->evalAt(x);
    
    return _interpolator->evalAt(x);
  }
  
  // keyframing 
  inline void addKeyframe(const Vector2f &pt)                  {_interpolator->addKeyframe(pt);  }
  inline void setKeyframe(const Vector2f &pt,unsigned int i=0) {_interpolator->setKeyframe(pt,i);}
  inline void setTangent(const Vector2f &pt,unsigned int i=0) {_interpolator->setTangent(pt,i);}
  inline void delKeyframe(unsigned int i)                      {_interpolator->delKeyframe(i);   }

  // curve parameters
  void setInterpolation(int interpolation);
  void setLeftBehavior (int behavior     );
  void setRightBehavior(int behavior     );

  // curve samples 
  const std::vector<Vector2f> samplePoints(float x1,float x2,unsigned int nb=100) const;
  const std::vector<Vector2f> sampleLines (float x1,float x2,unsigned int nb=100) const;

  // accessors 
  inline int interpType       () const {return _interpType;}
  inline int leftBehaviorType () const {return _leftType;  }
  inline int rightBehaviorType() const {return _rightType; }

  inline const QString &interpName        () const {return _interpNames[_interpType]; }
  inline const QString &leftBehaviorName  () const {return _behaviorNames[_leftType]; }
  inline const QString &rightBehaviorName () const {return _behaviorNames[_rightType];}

  inline unsigned int nbPoints()                   const {return _interpolator->nbPoints();  }
  inline unsigned int nbTangents()                 const {return _interpolator->nbTangents();}
  inline const std::vector<Vector2f> &points  ()   const {return _interpolator->points();    }
  inline const std::vector<Vector2f> &tangents()   const {return _interpolator->tangents();  }
  inline const Vector2f &point  (unsigned int i=0) const {return _interpolator->point(i);    }
  inline const Vector2f &tangent(unsigned int i=0) const {return _interpolator->tangent(i);  }

  inline bool useTangents() const {return _interpolator->useTangents();}

  static const QStringList &interpNames()   {return _interpNames;}
  static const QStringList &baheviorNames() {return _behaviorNames;}

  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

 private:
  CurveInterpolator *createInterpolator(int interpolation,const Vector2f &pt);
  CurveInterpolator *createInterpolator(int interpolation,CurveInterpolator *curve);
  CurveBehavior     *createBehavior(int side,int behavior);

  int _interpType;
  int _leftType;
  int _rightType;

  CurveInterpolator *_interpolator;
  CurveBehavior     *_leftBehavior;
  CurveBehavior     *_rightBehavior;

  static QStringList _interpNames;
  static QStringList _behaviorNames;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};


#endif // CURVE
