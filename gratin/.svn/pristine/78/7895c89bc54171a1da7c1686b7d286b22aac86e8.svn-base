// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef WIDGET_PARAMS
#define WIDGET_PARAMS

#include "misc/extinclude.h"
#include "misc/trackballCamera.h"
#include "core/keyframedParams.h"
#include "core/nodeInterface.h"
#include "view/nodeWidget.h"
#include "view/animationWidget.h"

#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QString>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QColorDialog>

// These classes provide a set of built-in widgets that can be directly included in user-defined node widgets 
// available classes are: 
// SliderFloat : a simple slider using floating values 
// SliderInt   : a simple slider using integer values 
// LabelFloat  : widget displaying a float value using labels 
// Label2Float : widget displaying a 2D float vector using labels 
// Label3Float : widget displaying a 3D float vector using labels 
// EditFloat   : widget displaying a float value editable using a spin box 
// Edit2Float  : widget displaying a 2D float vector editable using spin boxes 
// Edit3Float  : widget displaying a 3D float vector editable using spin boxes 
// EditInt     : widget displaying a integer value editable using a spin box 
// Edit2Int    : widget displaying a 2D integer vector editable using spin boxes 
// Edit3Int    : widget displaying a 3D integer vector editable using spin boxes 
// TrackballWidget : widget displaying camera parameters (controlable via mouse)
// ColorPicker : widget displaying a color picker dialog
// ComboBoxInt : widget displaying a comboBox

// adding KF in front of all of these classes allow the corresponding variables to
// be controlable with keyframes (it adds the 'set' and 'edit' buttons for animating them):
// KFSliderFloat
// KFSliderInt 
// KFLabelFloat
// KFLabel2Float
// KFLabel3Float
// KFEditFloat  
// KFEdit2Float 
// KFEdit3Float 
// KFEditInt    
// KFEdit2Int   
// KFEdit3Int   
// KFTrackballWidget
// KFColorPicker
// KFComboBoxInt

// A user may also create its own (keyframed) widget by inheriting NodeWidget (in view/nodeWidget.h)



// *****************************************************
// ***************** SliderFloat class *****************
// *****************************************************
class LIBRARY_EXPORT SliderFloat : public NodeWidget {
  Q_OBJECT

    public:
  SliderFloat(NodeInterface *node,const QString &name,float minVal=0.0f,float maxVal=1.0f,float value=0.0f,int precision=3);

  inline QString name()   const {return _nameLabel->text();}
  inline float   val()    const {return _currentVal;       }
  inline float   minVal() const {return _minVal;           }
  inline float   maxVal() const {return _maxVal;           }

  inline void setVal(float v,bool update=true) {
    _update = update;
    _slider->setValue(floatToInt(v));
    _update = true;
  }

  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

 signals:
  void valChanged();

  private slots:
  void valueChanged(int);

 protected:
  inline float intToFloat(int   v) {return (((float)v/_scale)*(_maxVal-_minVal))+_minVal;}
  inline int   floatToInt(float v) {return (int)(((v-_minVal)/(_maxVal-_minVal))*_scale);}
  
  float    _maxVal;
  float    _minVal;
  float    _currentVal;
  float    _scale;
  bool     _update;
  int      _precision;

  QSlider *_slider;
  QLabel  *_nameLabel;
  QLabel  *_valLabel;
};


// *****************************************************
// **************** KFSliderFloat class ****************
// *****************************************************
class LIBRARY_EXPORT KFSliderFloat : public SliderFloat {
  Q_OBJECT

 public:
  KFSliderFloat(NodeInterface *node,const QString &name,float minVal=0.0f,float maxVal=1.0f,float value=0.0f,int precision=3);

  inline virtual bool isKeyframed() const {return true;}
  inline virtual void computeNewFrameValues();
  inline virtual bool currentValuesHaveChanged();

  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

  public slots:
  void editClicked();
  void setClicked();
  void checkChangeCurve();

 private:
  KeyframedFloat _kf;
  QPushButton *_editButton;
  QPushButton *_setButton;
};

inline void KFSliderFloat::computeNewFrameValues() {
  const float current = (float)AnimationWidget::instance()->currentFrame();
  _kf.frameChanged(current);
}

inline bool KFSliderFloat::currentValuesHaveChanged() {
  const float tmp = val();

  if(tmp!=_kf.get()) {
    _currentVal = _kf.get();
    return true;
  }

  return false;
}


// *****************************************************
// ****************** SliderInt class ******************
// *****************************************************
class LIBRARY_EXPORT SliderInt : public NodeWidget {
  Q_OBJECT

    public:
  SliderInt(NodeInterface *node,const QString &name,int minVal=0,int maxVal=100,int value=0);

  inline QString name()   const {return _nameLabel->text();}
  inline int     val()    const {return _currentVal;       }
  inline int     minVal() const {return _minVal;           }
  inline int     maxVal() const {return _maxVal;           }

  inline void setVal(int v,bool update=true) {
    _update = update;
    _slider->setValue(v);
    _update = true;
  }

  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

 signals:
  void valChanged();

  private slots:
  void valueChanged(int);

 protected:
  int    _maxVal;
  int    _minVal;
  int    _currentVal;
  bool   _update;

  QSlider *_slider;
  QLabel  *_nameLabel;
  QLabel  *_valLabel;
};



// *****************************************************
// ***************** KFSliderInt class *****************
// *****************************************************
class LIBRARY_EXPORT KFSliderInt : public SliderInt {
  Q_OBJECT

 public:
  KFSliderInt(NodeInterface *node,const QString &name,int minVal=0,int maxVal=100,int value=0);

  inline virtual bool isKeyframed() const {return true;}
  inline virtual void computeNewFrameValues();
  inline virtual bool currentValuesHaveChanged();

  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

  public slots:
  void editClicked();
  void setClicked();
  void checkChangeCurve();

 private:
  KeyframedFloat _kf;
  QPushButton *_editButton;
  QPushButton *_setButton;
};

inline void KFSliderInt::computeNewFrameValues() {
  const float current = (float)AnimationWidget::instance()->currentFrame();
  _kf.frameChanged(current);
}

inline bool KFSliderInt::currentValuesHaveChanged() {
  const int tmp = (int)val();

  if(tmp!=(int)_kf.get()) {
    _currentVal = (int)_kf.get();
    return true;
  }

  return false;
}



// *****************************************************
// ****************** LabelFloat class *****************
// *****************************************************
class LIBRARY_EXPORT LabelFloat : public NodeWidget {
  Q_OBJECT

    public:
  LabelFloat(NodeInterface *node,const QString &name,float value=0.0f,int precision=3);

  inline QString name() const {return _nameLabel->text();}
  inline float val() const {return _currentVal;}
  inline void setVal(float v,bool update=true) {
    _currentVal = v;
    _valLabel->setText(QString::number(_currentVal,'f',_precision));
    if(update) updateGraph();
  }

  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

 protected:
  float _currentVal;
  int   _precision;

  QLabel  *_nameLabel;
  QLabel  *_valLabel;
};


// *****************************************************
// ***************** KFLabelFloat class ****************
// *****************************************************
class LIBRARY_EXPORT KFLabelFloat : public LabelFloat {
  Q_OBJECT

 public:
  KFLabelFloat(NodeInterface *node,const QString &name,float value=0.0f,float maxAbsVal=10000,int precision=3);

  inline virtual bool isKeyframed() const {return true;}
  inline virtual void computeNewFrameValues();
  inline virtual bool currentValuesHaveChanged();

  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

  public slots:
  void editClicked();
  void setClicked();

 private:
  KeyframedFloat _kf;
  float _maxAbsVal;

  QPushButton *_editButton;
  QPushButton *_setButton;
};

inline void KFLabelFloat::computeNewFrameValues() {
  const float current = (float)AnimationWidget::instance()->currentFrame();
  _kf.frameChanged(current);
}

inline bool KFLabelFloat::currentValuesHaveChanged() {
  const float tmp = val();
  const float current = _kf.get();

  if(tmp!=current) {
    _currentVal = current;
    return true;
  }

  return false;
}


// *****************************************************
// ***************** Label2Float class *****************
// *****************************************************
class LIBRARY_EXPORT Label2Float : public NodeWidget {
  Q_OBJECT

    public:
  Label2Float(NodeInterface *node,const QString &name,const Vector2f &value=Vector2f(0,0),int precision=3);

  inline QString name() const {return _nameLabel->text();}
  inline const Vector2f val() const {return _currentVal;}
  inline void setVal(const Vector2f &v,bool update=true) {
    _currentVal = v;
    _val1Label->setText(QString::number(_currentVal[0],'f',_precision));
    _val2Label->setText(QString::number(_currentVal[1],'f',_precision));
    if(update) updateGraph();
  }

  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

 protected:
  Vector2f _currentVal;
  int      _precision;

  QLabel  *_nameLabel;
  QLabel  *_val1Label;
  QLabel  *_val2Label;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};


// *****************************************************
// **************** KFLabel2Float class ****************
// *****************************************************
class LIBRARY_EXPORT KFLabel2Float : public Label2Float {
  Q_OBJECT

 public:
  KFLabel2Float(NodeInterface *node,const QString &name,const Vector2f &value=Vector2f(0,0),float maxAbsVal=10000,int precision=3);

  inline virtual bool isKeyframed() const {return true;}
  inline virtual void computeNewFrameValues();
  inline virtual bool currentValuesHaveChanged();

  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

  public slots:
  void editClicked();
  void setClicked();

 private:
  KeyframedFloat _kfX;
  KeyframedFloat _kfY;
  float _maxAbsVal;

  QPushButton *_editButton;
  QPushButton *_setButton;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

inline void KFLabel2Float::computeNewFrameValues() {
  const float current = (float)AnimationWidget::instance()->currentFrame();
  _kfX.frameChanged(current);
  _kfY.frameChanged(current);
}

inline bool KFLabel2Float::currentValuesHaveChanged() {
  const Vector2f tmp = val();
  const Vector2f current = Vector2f(_kfX.get(),_kfY.get());

  if(tmp!=current) {
    _currentVal = current;
    return true;
  }

  return false;
}


// *****************************************************
// ***************** Label3Float class *****************
// *****************************************************
class LIBRARY_EXPORT Label3Float : public NodeWidget {
  Q_OBJECT

    public:
  Label3Float(NodeInterface *node,const QString &name,const Vector3f &value=Vector3f(0,0,0),int precision=3);

  inline QString name() const {return _nameLabel->text();}
  inline const Vector3f val() const {return _currentVal;}
  inline void setVal(const Vector3f &v,bool update=true) {
    _currentVal = v;
    _val1Label->setText(QString::number(_currentVal[0],'f',_precision));
    _val2Label->setText(QString::number(_currentVal[1],'f',_precision));
    _val3Label->setText(QString::number(_currentVal[2],'f',_precision));
    if(update) updateGraph();
  }

  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

 protected:
  Vector3f _currentVal;
  int      _precision;

  QLabel  *_nameLabel;
  QLabel  *_val1Label;
  QLabel  *_val2Label;
  QLabel  *_val3Label;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};


// *****************************************************
// **************** KFLabel3Float class ****************
// *****************************************************
class LIBRARY_EXPORT KFLabel3Float : public Label3Float {
  Q_OBJECT

 public:
  KFLabel3Float(NodeInterface *node,const QString &name,const Vector3f &value=Vector3f(0,0,0),float maxAbsVal=10000,int precision=3);

  inline virtual bool isKeyframed() const {return true;}
  inline virtual void computeNewFrameValues();
  inline virtual bool currentValuesHaveChanged();

  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

  public slots:
  void editClicked();
  void setClicked();

 private:
  KeyframedFloat _kfX;
  KeyframedFloat _kfY;
  KeyframedFloat _kfZ;
  float _maxAbsVal;

  QPushButton *_editButton;
  QPushButton *_setButton;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

inline void KFLabel3Float::computeNewFrameValues() {
  const float current = (float)AnimationWidget::instance()->currentFrame();
  _kfX.frameChanged(current);
  _kfY.frameChanged(current);
  _kfZ.frameChanged(current);
}

inline bool KFLabel3Float::currentValuesHaveChanged() {
  const Vector3f tmp = val();
  const Vector3f current = Vector3f(_kfX.get(),_kfY.get(),_kfZ.get());

  if(tmp!=current) {
    _currentVal = current;
    return true;
  }

  return false;
}


// *****************************************************
// ****************** EditFloat class ******************
// *****************************************************
class LIBRARY_EXPORT EditFloat : public NodeWidget {
  Q_OBJECT

    public:
  EditFloat(NodeInterface *node,const QString &name,float minVal=0.0f,float maxVal=1.0f,float value=0.0f);

  inline QString name()   const {return _nameLabel->text();}
  inline float   val()    const {return _currentVal;       }
  inline float   minVal() const {return _minVal;           }
  inline float   maxVal() const {return _maxVal;           }

  inline void setVal(float v,bool update=true) {
    _update = update;
    _edit->setValue(v);
    _update = true;
  }

  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

 signals:
  void valChanged();

  private slots:
  void valueChanged(double);

 protected:
  float    _maxVal;
  float    _minVal;
  float    _currentVal;
  bool     _update;

  QDoubleSpinBox *_edit;
  QLabel  *_nameLabel;
};

// *****************************************************
// ***************** KFEditFloat class *****************
// *****************************************************
class LIBRARY_EXPORT KFEditFloat : public EditFloat {
  Q_OBJECT

 public:
  KFEditFloat(NodeInterface *node,const QString &name,float minVal=0.0f,float maxVal=1.0f,float value=0.0f);

  inline virtual bool isKeyframed() const {return true;}
  inline virtual void computeNewFrameValues();
  inline virtual bool currentValuesHaveChanged();

  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

  public slots:
  void editClicked();
  void setClicked();
  void checkChangeCurve();

 private:
  KeyframedFloat _kf;
  QPushButton *_editButton;
  QPushButton *_setButton;
};

inline void KFEditFloat::computeNewFrameValues() {
  const float current = (float)AnimationWidget::instance()->currentFrame();
  _kf.frameChanged(current);
}

inline bool KFEditFloat::currentValuesHaveChanged() {
  const float tmp = val();

  if(tmp!=_kf.get()) {
    _currentVal = _kf.get();
    return true;
  }

  return false;
}


// *****************************************************
// ***************** Edit2Float class ******************
// *****************************************************
class LIBRARY_EXPORT Edit2Float : public NodeWidget {
  Q_OBJECT

    public:
  Edit2Float(NodeInterface *node,const QString &name,const Vector2f &minVal=Vector2f(0,0),const Vector2f &maxVal=Vector2f(1,1),const Vector2f &value=Vector2f(0,0));

  inline QString  name()   const {return _nameLabel->text();}
  inline Vector2f val()    const {return _currentVal;       }
  inline Vector2f minVal() const {return _minVal;           }
  inline Vector2f maxVal() const {return _maxVal;           }

  inline void setVal(const Vector2f &v,bool update=true) {
    _update = false;
    _edit1->setValue(v[0]);
    _edit2->setValue(v[1]);

    if(update && v!=_currentVal) {
      updateGraph();
    }

    _update = true;
  }

  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

 signals:
  void valChanged();

  private slots:
  void value1Changed(double);
  void value2Changed(double);

 protected:
  Vector2f _maxVal;
  Vector2f _minVal;
  Vector2f _currentVal;
  bool     _update;

  QDoubleSpinBox *_edit1;
  QDoubleSpinBox *_edit2;
  QLabel  *_nameLabel;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

// *****************************************************
// ***************** KFEdit2Float class ****************
// *****************************************************
class LIBRARY_EXPORT KFEdit2Float : public Edit2Float {
  Q_OBJECT

 public:
  KFEdit2Float(NodeInterface *node,const QString &name,const Vector2f &minVal=Vector2f(0,0),const Vector2f &maxVal=Vector2f(1,1),const Vector2f &value=Vector2f(0,0));

  inline virtual bool isKeyframed() const {return true;}
  inline virtual void computeNewFrameValues();
  inline virtual bool currentValuesHaveChanged();

  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

  public slots:
  void editClicked();
  void setClicked();
  void checkChangeCurve();

 private:
  KeyframedFloat _kfX;
  KeyframedFloat _kfY;
  QPushButton *_editButton;
  QPushButton *_setButton;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

inline void KFEdit2Float::computeNewFrameValues() {
  const float current = (float)AnimationWidget::instance()->currentFrame();
  _kfX.frameChanged(current);
  _kfY.frameChanged(current);
}

inline bool KFEdit2Float::currentValuesHaveChanged() {
  const Vector2f oldv = val();
  const Vector2f newv = Vector2f(_kfX.get(),_kfY.get());

  if(oldv!=newv) {
    _currentVal = newv;
    return true;
  }

  return false;
}


// *****************************************************
// ***************** Edit3Float class ******************
// *****************************************************
class LIBRARY_EXPORT Edit3Float : public NodeWidget {
  Q_OBJECT

    public:
  Edit3Float(NodeInterface *node,const QString &name,const Vector3f &minVal=Vector3f(0,0,0),const Vector3f &maxVal=Vector3f(1,1,1),const Vector3f &value=Vector3f(0,0,0));

  inline QString  name()   const {return _nameLabel->text();}
  inline Vector3f val()    const {return _currentVal;       }
  inline Vector3f minVal() const {return _minVal;           }
  inline Vector3f maxVal() const {return _maxVal;           }

  inline void setVal(const Vector3f &v,bool update=true) {
    _update = false;
    _edit1->setValue(v[0]);
    _edit2->setValue(v[1]);
    _edit3->setValue(v[2]);

    if(update && v!=_currentVal) {
      updateGraph();
    }

    _update = true;
  }

  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

 signals:
  void valChanged();

  private slots:
  void value1Changed(double);
  void value2Changed(double);
  void value3Changed(double);

 protected:
  Vector3f _maxVal;
  Vector3f _minVal;
  Vector3f _currentVal;
  bool     _update;

  QDoubleSpinBox *_edit1;
  QDoubleSpinBox *_edit2;
  QDoubleSpinBox *_edit3;
  QLabel  *_nameLabel;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};


// *****************************************************
// ***************** KFEdit3Float class ****************
// *****************************************************
class LIBRARY_EXPORT KFEdit3Float : public Edit3Float {
  Q_OBJECT

 public:
  KFEdit3Float(NodeInterface *node,const QString &name,const Vector3f &minVal=Vector3f(0,0,0),const Vector3f &maxVal=Vector3f(1,1,1),const Vector3f &value=Vector3f(0,0,0));

  inline virtual bool isKeyframed() const {return true;}
  inline virtual void computeNewFrameValues();
  inline virtual bool currentValuesHaveChanged();

  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

  public slots:
  void editClicked();
  void setClicked();
  void checkChangeCurve();

 private:
  KeyframedFloat _kfX;
  KeyframedFloat _kfY;
  KeyframedFloat _kfZ;
  QPushButton *_editButton;
  QPushButton *_setButton;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

inline void KFEdit3Float::computeNewFrameValues() {
  const float current = (float)AnimationWidget::instance()->currentFrame();
  _kfX.frameChanged(current);
  _kfY.frameChanged(current);
  _kfZ.frameChanged(current);
}

inline bool KFEdit3Float::currentValuesHaveChanged() {
  const Vector3f oldv = val();
  const Vector3f newv = Vector3f(_kfX.get(),_kfY.get(),_kfZ.get());

  if(oldv!=newv) {
    _currentVal = newv;
    return true;
  }

  return false;
}



// *****************************************************
// ****************** EditInt class ******************
// *****************************************************
class LIBRARY_EXPORT EditInt : public NodeWidget {
  Q_OBJECT

    public:
  EditInt(NodeInterface *node,const QString &name,int minVal=0,int maxVal=100,int value=0);

  inline QString name() const {return _nameLabel->text();}
  inline int   val()    const {return _currentVal;       }
  inline int minVal()   const {return _minVal;           }
  inline int maxVal()   const {return _maxVal;           }

  inline void setVal(int v,bool update=true) {
    _update = update;
    _edit->setValue(v);
    _update = true;
  }

  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

 signals:
  void valChanged();

  private slots:
  void valueChanged(int);

 protected:
  int    _maxVal;
  int    _minVal;
  int    _currentVal;
  bool     _update;

  QSpinBox *_edit;
  QLabel  *_nameLabel;
};

// *****************************************************
// ***************** KFEditInt class *****************
// *****************************************************
class LIBRARY_EXPORT KFEditInt : public EditInt {
  Q_OBJECT

 public:
  KFEditInt(NodeInterface *node,const QString &name,int minVal=0,int maxVal=100,int value=0);

  inline virtual bool isKeyframed() const {return true;}
  inline virtual void computeNewFrameValues();
  inline virtual bool currentValuesHaveChanged();

  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

  public slots:
  void editClicked();
  void setClicked();
  void checkChangeCurve();

 private:
  KeyframedFloat _kf;
  QPushButton *_editButton;
  QPushButton *_setButton;
};

inline void KFEditInt::computeNewFrameValues() {
  const float current = (float)AnimationWidget::instance()->currentFrame();
  _kf.frameChanged(current);
}

inline bool KFEditInt::currentValuesHaveChanged() {
  const int tmp = val();

  if(tmp!=(int)_kf.get()) {
    _currentVal = (int)_kf.get();
    return true;
  }

  return false;
}


// *****************************************************
// ***************** Edit2Int class ******************
// *****************************************************
class LIBRARY_EXPORT Edit2Int : public NodeWidget {
  Q_OBJECT

    public:
  Edit2Int(NodeInterface *node,const QString &name,const Vector2i &minVal=Vector2i(0,0),const Vector2i &maxVal=Vector2i(100,100),const Vector2i &value=Vector2i(0,0));

  inline QString  name()   const {return _nameLabel->text();}
  inline Vector2i val()    const {return _currentVal;       }
  inline Vector2i minVal() const {return _minVal;           }
  inline Vector2i maxVal() const {return _maxVal;           }

  inline void setVal(const Vector2i &v,bool update=true) {
    _update = false;
    _edit1->setValue(v[0]);
    _edit2->setValue(v[1]);
    if(update && v!=_currentVal) {
      updateGraph();
    }
    _update = true;
  }

  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

 signals:
  void valChanged();

  private slots:
  void value1Changed(int);
  void value2Changed(int);

 protected:
  Vector2i _maxVal;
  Vector2i _minVal;
  Vector2i _currentVal;
  bool     _update;

  QSpinBox *_edit1;
  QSpinBox *_edit2;
  QLabel  *_nameLabel;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

// *****************************************************
// ***************** KFEdit2Int class ****************
// *****************************************************
class LIBRARY_EXPORT KFEdit2Int : public Edit2Int {
  Q_OBJECT

 public:
  KFEdit2Int(NodeInterface *node,const QString &name,const Vector2i &minVal=Vector2i(0,0),const Vector2i &maxVal=Vector2i(100,100),const Vector2i &value=Vector2i(0,0));

  inline virtual bool isKeyframed() const {return true;}
  inline virtual void computeNewFrameValues();
  inline virtual bool currentValuesHaveChanged();

  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

  public slots:
  void editClicked();
  void setClicked();
  void checkChangeCurve();

 private:
  KeyframedFloat _kfX;
  KeyframedFloat _kfY;
  QPushButton *_editButton;
  QPushButton *_setButton;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

inline void KFEdit2Int::computeNewFrameValues() {
  const float current = (float)AnimationWidget::instance()->currentFrame();
  _kfX.frameChanged(current);
  _kfY.frameChanged(current);
}

inline bool KFEdit2Int::currentValuesHaveChanged() {
  const Vector2i oldv = val();
  const Vector2i newv = Vector2i((int)_kfX.get(),(int)_kfY.get());

  if(oldv!=newv) {
    _currentVal = newv;
    return true;
  }

  return false;
}


// *****************************************************
// ***************** Edit3Int class ******************
// *****************************************************
class LIBRARY_EXPORT Edit3Int : public NodeWidget {
  Q_OBJECT

    public:
  Edit3Int(NodeInterface *node,const QString &name,const Vector3i &minVal=Vector3i(0,0,0),const Vector3i &maxVal=Vector3i(100,100,100),const Vector3i &value=Vector3i(0,0,0));

  inline QString  name()   const {return _nameLabel->text();}
  inline Vector3i val()    const {return _currentVal;       }
  inline Vector3i minVal() const {return _minVal;           }
  inline Vector3i maxVal() const {return _maxVal;           }

  inline void setVal(const Vector3i &v,bool update=true) {
    _update = false;
    _edit1->setValue(v[0]);
    _edit2->setValue(v[1]);
    _edit3->setValue(v[2]);
    if(update && v!=_currentVal) {
      updateGraph();
    }
    _update = true;
  }

  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

 signals:
  void valChanged();

  private slots:
  void value1Changed(int);
  void value2Changed(int);
  void value3Changed(int);

 protected:
  Vector3i _maxVal;
  Vector3i _minVal;
  Vector3i _currentVal;
  bool     _update;

  QSpinBox *_edit1;
  QSpinBox *_edit2;
  QSpinBox *_edit3;
  QLabel  *_nameLabel;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};


// *****************************************************
// ***************** KFEdit3Int class ****************
// *****************************************************
class LIBRARY_EXPORT KFEdit3Int : public Edit3Int {
  Q_OBJECT

 public:
  KFEdit3Int(NodeInterface *node,const QString &name,const Vector3i &minVal=Vector3i(0,0,0),const Vector3i &maxVal=Vector3i(100,100,100),const Vector3i &value=Vector3i(0,0,0));

  inline virtual bool isKeyframed() const {return true;}
  inline virtual void computeNewFrameValues();
  inline virtual bool currentValuesHaveChanged();

  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

  public slots:
  void editClicked();
  void setClicked();
  void checkChangeCurve();

 private:
  KeyframedFloat _kfX;
  KeyframedFloat _kfY;
  KeyframedFloat _kfZ;
  QPushButton *_editButton;
  QPushButton *_setButton;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

inline void KFEdit3Int::computeNewFrameValues() {
  const float current = (float)AnimationWidget::instance()->currentFrame();
  _kfX.frameChanged(current);
  _kfY.frameChanged(current);
  _kfZ.frameChanged(current);
}

inline bool KFEdit3Int::currentValuesHaveChanged() {
  const Vector3i oldv = val();
  const Vector3i newv = Vector3i((int)_kfX.get(),(int)_kfY.get(),(int)_kfZ.get());

  if(oldv!=newv) {
    _currentVal = newv;
    return true;
  }

  return false;
}



// *****************************************************
// *************** TrackbalCamera class ****************
// *****************************************************
class LIBRARY_EXPORT KFTrackballWidget : public NodeWidget {
  Q_OBJECT

    public:
  KFTrackballWidget(NodeInterface *node,const QString &name,TrackballCamera *cam);

  inline virtual bool isKeyframed() const {return true;}
  inline QString name()   const {return _nameLabel->text();}
  inline TrackballCamera *val() const {return _cam;        }
  inline void setVal(TrackballCamera *cam,bool update=true) {
    _update = update;
    _cam = cam;    
    _projection->setCurrentIndex(_cam->projType());
    _rotation->setCurrentIndex(_cam->rotType());
    _update = true;
  }
  inline virtual void computeNewFrameValues();
  inline virtual bool currentValuesHaveChanged();

  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

  public slots:
  void projectionChanged(int);
  void rotationChanged(int);
  void editClicked();
  void setClicked();
  void checkChangeCurve();

 protected:  
  TrackballCamera *_cam;
  QLabel    *_nameLabel;
  QComboBox *_projection;
  QComboBox *_rotation;
  bool       _update;

  KeyframedFloat _kfShiftX;
  KeyframedFloat _kfShiftY;
  KeyframedFloat _kfShiftZ;
  KeyframedFloat _kfQuatX;
  KeyframedFloat _kfQuatY;
  KeyframedFloat _kfQuatZ;
  KeyframedFloat _kfQuatW;
  QPushButton *_editButton;
  QPushButton *_setButton;
};


inline void KFTrackballWidget::computeNewFrameValues() {
  const float current = (float)AnimationWidget::instance()->currentFrame();
  _kfShiftX.frameChanged(current);
  _kfShiftY.frameChanged(current);
  _kfShiftZ.frameChanged(current);
  _kfQuatX.frameChanged(current);
  _kfQuatY.frameChanged(current);
  _kfQuatZ.frameChanged(current);
  _kfQuatW.frameChanged(current);
}

inline bool KFTrackballWidget::currentValuesHaveChanged() {
  TrackballCamera *cam = val();

  const Quaternionf oldQuat = cam->rotation();
  const Quaternionf newQuat = Quaternionf(_kfQuatW.get(),_kfQuatX.get(),_kfQuatY.get(),_kfQuatZ.get()).normalized();
  const Vector3f oldShift   = cam->shift();
  const Vector3f newShift(_kfShiftX.get(),_kfShiftY.get(),_kfShiftZ.get());

  const bool sameQuat = (oldQuat.x()==newQuat.x() && 
			 oldQuat.y()==newQuat.y() && 
			 oldQuat.z()==newQuat.z() && 
			 oldQuat.w()==newQuat.w());

  if(oldShift!=newShift || !sameQuat) {
    _cam->setShift(newShift);
    _cam->setRotation(newQuat);
    return true;
  }

  return false;
}





// *****************************************************
// ***************** ColorPicker class ******************
// *****************************************************
class LIBRARY_EXPORT ColorPicker : public NodeWidget {
  Q_OBJECT

    public:
  ColorPicker(NodeInterface *node,const QString &name,const Vector4f &value=Vector4f(1,1,1,1));

  inline QString  name()   const {return _nameLabel->text();}
  inline Vector4f val()    const {return _currentVal;       }

  inline void setVal(const Vector4f &v,bool update=true) {
    _update = update;
    _colorPicker->setCurrentColor(QColor((int)(v[0]*255.0f),
					 (int)(v[1]*255.0f),
					 (int)(v[2]*255.0f),
					 (int)(v[3]*255.0f)));
    _currentVal = v;
    _update = true;
  }

  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

 signals:
  void valChanged();

  private slots:
  void colorChanged(const QColor &color);

 protected:
  Vector4f      _currentVal;
  bool          _update;
  QLabel       *_nameLabel;
  QColorDialog *_colorPicker;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};


// *****************************************************
// ***************** KFColorPicker class ****************
// *****************************************************
class LIBRARY_EXPORT KFColorPicker : public ColorPicker {
  Q_OBJECT

 public:
  KFColorPicker(NodeInterface *node,const QString &name,const Vector4f &value=Vector4f(1,1,1,1));

  inline virtual bool isKeyframed() const {return true;}
  inline virtual void computeNewFrameValues();
  inline virtual bool currentValuesHaveChanged();

  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

  public slots:
  void editClicked();
  void setClicked();
  void checkChangeCurve();

 private:
  KeyframedFloat _kfR;
  KeyframedFloat _kfG;
  KeyframedFloat _kfB;
  KeyframedFloat _kfA;
  QPushButton *_editButton;
  QPushButton *_setButton;
};

inline void KFColorPicker::computeNewFrameValues() {
  const float current = (float)AnimationWidget::instance()->currentFrame();
  _kfR.frameChanged(current);
  _kfG.frameChanged(current);
  _kfB.frameChanged(current);
  _kfA.frameChanged(current);
}

inline bool KFColorPicker::currentValuesHaveChanged() {
  const Vector4f oldv = val();
  const Vector4f newv = Vector4f(_kfR.get(),_kfG.get(),_kfB.get(),_kfA.get());

  if(oldv!=newv) {
    _currentVal = newv;
    return true;
  }

  return false;
}


// *****************************************************
// ****************** ComboBoxInt class ******************
// *****************************************************
class LIBRARY_EXPORT ComboBoxInt : public NodeWidget {
  Q_OBJECT

    public:
  ComboBoxInt(NodeInterface *node,const QString &name,const QStringList vals,int value=0);

  inline QString name() const {return _nameLabel->text();}
  inline int   val()    const {return _currentVal;       }
  inline int minVal()   const {return _minVal;           }
  inline int maxVal()   const {return _maxVal;           }

  inline void setVal(int v,bool update=true) {
    _update = update;
    _edit->setCurrentIndex(v);
    _update = true;
  }

  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

 signals:
  void valChanged();

  private slots:
  void valueChanged(int);

 protected:
  int    _maxVal;
  int    _minVal;
  int    _currentVal;
  bool   _update;

  QComboBox *_edit;
  QLabel    *_nameLabel;
};

// *****************************************************
// ***************** KFComboBoxInt class *****************
// *****************************************************
class LIBRARY_EXPORT KFComboBoxInt : public ComboBoxInt {
  Q_OBJECT

 public:
  KFComboBoxInt(NodeInterface *node,const QString &name,const QStringList vals,int value=0);

  inline virtual bool isKeyframed() const {return true;}
  inline virtual void computeNewFrameValues();
  inline virtual bool currentValuesHaveChanged();

  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

  public slots:
  void editClicked();
  void setClicked();
  void checkChangeCurve();

 private:
  KeyframedFloat _kf;
  QPushButton *_editButton;
  QPushButton *_setButton;
};

inline void KFComboBoxInt::computeNewFrameValues() {
  const float current = (float)AnimationWidget::instance()->currentFrame();
  _kf.frameChanged(current);
}

inline bool KFComboBoxInt::currentValuesHaveChanged() {
  const int tmp = val();

  if(tmp!=(int)_kf.get()) {
    _currentVal = (int)_kf.get();
    return true;
  }

  return false;
}




#endif // WIDGET_PARAMS
