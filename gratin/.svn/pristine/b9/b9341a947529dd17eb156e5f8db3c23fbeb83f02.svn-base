// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "core/keyframedParams.h"

KeyframedFloat::KeyframedFloat(float minVal,float maxVal,float defaultVal) 
  : _minVal(minVal),
    _maxVal(maxVal),
    _currentVal(defaultVal),
    _curve(new Curve(Vector2f(0,_currentVal))) {
  
}

KeyframedFloat::~KeyframedFloat() {
  delete _curve;
}

