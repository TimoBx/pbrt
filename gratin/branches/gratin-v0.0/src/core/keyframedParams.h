// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef KEYFRAMED_PARAMS
#define KEYFRAMED_PARAMS

#include "core/curve.h"
#include "core/sceneIOTags.h"

class KeyframedFloat : public IOData {
 public:
  KeyframedFloat(float minVal=0.0f,float maxVal=1.0f,float defaultVal=0.0f);
  ~KeyframedFloat();

  inline virtual float get() const;
  inline virtual void  set(float v);

  inline bool frameChanged(float x);
  inline Curve *curve() const;

  inline virtual bool save(SceneSaver  *saver );
  inline virtual bool load(SceneLoader *loader);

 private:
  float _minVal;
  float _maxVal;
  float _currentVal;
  
  Curve *_curve;  

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

inline float KeyframedFloat::get() const  {
  return _currentVal;
}

inline void  KeyframedFloat::set(float v) {
  _currentVal = v;
}

inline bool KeyframedFloat::frameChanged(float x) { 
  const float tmp = _curve->evalAt(x);

  // report that the new frame did not change the current variable
  if(_currentVal==tmp) return false;
  
  _currentVal = tmp;
  return true;
}

inline Curve *KeyframedFloat::curve() const {
  return _curve;
}

inline bool KeyframedFloat::save(SceneSaver *saver) {
  return (saver->saveFloat("minVal",_minVal) && 
	  saver->saveFloat("maxVal",_maxVal) && 
	  saver->saveFloat("currentVal",_currentVal) && 
	  saver->saveIOData("curve",_curve));
}

inline bool KeyframedFloat::load(SceneLoader *loader) {
  return (loader->loadFloat("minVal",_minVal) && 
	  loader->loadFloat("maxVal",_maxVal) && 
	  loader->loadFloat("currentVal",_currentVal) && 
	  loader->loadIOData("curve",_curve));
}


#endif // KEYFRAMED_PARAMS
