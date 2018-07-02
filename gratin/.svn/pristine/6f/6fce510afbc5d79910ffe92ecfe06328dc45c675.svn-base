// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef TIMELINE_ITEMS_H
#define TIMELINE_ITEMS_H

#include <QGraphicsItem>

#include "misc/glinclude.h"
#include "core/curve.h"
#include "core/animation.h"
#include "view/graphWidget.h"
#include "view/timelineWidget.h"
#include "view/animationWidget.h"

// implemented items:
// * CurveDrawer: draw the curves 
// * Boundary drawer: draw the min and max boundaries of the current param
// * Position drawer: write the current mouse position 
// * ControlPoint: draw one control point for a given curve
// * TangentPoint: draw one tangent point for a given curve
// * TangentLine: draw the link between one control point and its tangent
// * Frame: draw the current frame 


// Item fur curves 
class CurveDrawer : public QGraphicsItem {
 public:
 CurveDrawer(TimelineWidget *timelineWidget,TimelineWidget::Param *p) : _timelineWidget(timelineWidget),_param(p) {}

 protected:
  QRectF boundingRect() const;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
  
 private:
  TimelineWidget *_timelineWidget;
  TimelineWidget::Param *_param;
};





// Item fur curve limits 
class BoundaryDrawer : public QGraphicsItem {
 public:
 BoundaryDrawer(TimelineWidget *timelineWidget) : _timelineWidget(timelineWidget),_minVal(0),_maxVal(0) {  
    setZValue(-2);
  }

  inline void setLimits(float minVal,float maxVal) {_minVal = minVal;_maxVal = maxVal;}

 protected:
  QRectF boundingRect() const;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

 private:
  TimelineWidget *_timelineWidget;
  float _minVal;
  float _maxVal;
};




// Item for drawing frame and mouse position
class PositionDrawer : public QGraphicsItem {
 public:
  PositionDrawer(TimelineWidget *timelineWidget,unsigned int f,float x,float y) 
    : _timelineWidget(timelineWidget), _f(f), _x(x), _y(y) {
    setFlag(ItemIgnoresTransformations); setZValue(2);
    computePos();
  }

  inline void computePos();
  inline void setPosition(float x,float y) {_x=x;_y=y;}
  //inline void setFrame(unsigned int f    ) {_f=f;}

 protected:
  QRectF boundingRect() const;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

 private:
  TimelineWidget *_timelineWidget;
  unsigned int _f;
  float _x;
  float _y;
  static const int _size = 300;
};

inline void PositionDrawer::computePos() {
  const QPointF p = _timelineWidget->mapToScene(QPoint(_timelineWidget->width()-_size-10,0));
  setPos(p);
}

// Item for control tangents 
class TangentPoint : public QGraphicsItem {
 public:
  TangentPoint(TimelineWidget *timelineWidget,TimelineWidget::Param *p,unsigned int index);
  inline const QStringList &varName() const {return _param->varName;}
  inline unsigned int index()         const {return _index;}
  inline void setIndex(unsigned int index) {_index = index;}
  inline void updatePos();
 
 protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseMoveEvent (QGraphicsSceneMouseEvent * event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
  QVariant itemChange(GraphicsItemChange change, const QVariant &value);
  QRectF boundingRect() const;

 private:
  inline QPointF computePos() const;
  inline float closestVal(float x,float s) const;

  TimelineWidget        *_timelineWidget;
  TimelineWidget::Param *_param;
  unsigned int           _index;
  bool                   _ctrl;
  bool                   _shift;
  float                  _mag;
  static const int       _size = 5;
};

inline float TangentPoint::closestVal(float x,float s) const {
  const float m = fmod(x,s);
  return m<s/2.0f ? x-m : x-m+s;
}

// TODO: align tangent with viewport
inline QPointF TangentPoint::computePos() const {
  const Vector2f pt  = _param->curve->point(_index);
  const Vector2f ta  = _param->curve->tangent(_index).normalized();
  const QPointF  l = _timelineWidget->mapToScene(QPoint(_mag,_mag)) - _timelineWidget->mapToScene(QPoint(0,0));
  //const float m = fabs(ta[0]);
  //const float d = m*l.x()+(1.0f-m)*l.y();
  //const float d = Vector2f(ta[0]*fabs(l.x()),ta[1]*fabs(l.y())).norm();
  //std::cout << "l = " << l.x() << ", " << l.y() << " -- m = " << m << " -- d = " << d << std::endl;
  return QPointF(pt[0]+ta[0]*fabs(l.x()),pt[1]+ta[1]*fabs(l.y()));
  //return QPointF(pt[0]+ta[0]*d,pt[1]+ta[1]*d);
}

inline void TangentPoint::updatePos() {
  setPos(computePos());
}


// Item for control points 
class ControlPoint : public QGraphicsItem {
 public:
  ControlPoint(TimelineWidget *timelineWidget,TimelineWidget::Param *p,unsigned int index,TangentPoint *t=NULL);
  inline const QStringList &varName() const {return _param->varName;}
  inline unsigned int index()         const {return _index;}
  inline void setIndex(unsigned int index) {_index = index;}
 protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseMoveEvent (QGraphicsSceneMouseEvent * event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
  QVariant itemChange(GraphicsItemChange change, const QVariant &value);
  QRectF boundingRect() const;

 private:
  inline float closestVal(float x,float s) const;

  TimelineWidget        *_timelineWidget;
  TimelineWidget::Param *_param;
  unsigned int           _index;
  bool                   _ctrl;
  bool                   _shift;
  TangentPoint          *_tangent;
  static const int       _size = 8;
};

inline float ControlPoint::closestVal(float x,float s) const {
  const float m = fmod(x,s);
  return m<s/2.0f ? x-m : x-m+s;
}



// Item for drawing links between points and tangents 
class TangentLine : public QGraphicsItem {
 public:
  TangentLine(TimelineWidget::Param *param,ControlPoint *p,TangentPoint *t);

 protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
  QRectF boundingRect() const;

 private:
  TimelineWidget::Param *_param;
  ControlPoint *_p;
  TangentPoint *_t;
};


// Item for current frame
class Frame : public QGraphicsItem {
 public:
  Frame(TimelineWidget *timelineWidget,AnimationWidget *animWidget,GraphWidget *graphWidget);
  
  inline void computePos();
  inline void updatePos();
  inline void updateFrame();

 protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
  void mousePressEvent  (QGraphicsSceneMouseEvent *event);
  void mouseMoveEvent   (QGraphicsSceneMouseEvent * event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

  QVariant itemChange(GraphicsItemChange change, const QVariant &value);
  QRectF   boundingRect() const;

 private:
  TimelineWidget  *_timelineWidget;
  AnimationWidget *_animWidget;
  GraphWidget     *_graphWidget;
  Animation       *_animation;
};

inline void Frame::updateFrame() {
  setPos(QPointF((double)_animation->currentFrame(),pos().y()));
}

inline void Frame::computePos() {
  const QPointF p1 = mapFromScene(QPointF((double)_animation->currentFrame(),0.0));
  const QPointF p2 = _timelineWidget->mapToScene(QPoint(0,0));
  setPos(p1.x(),p2.y());
}

inline void Frame::updatePos() {
  const QPointF m = _timelineWidget->mapToScene(QPoint(0,0));
  QPointF p = pos();
  p.setY(m.y());
  setPos(p);
}


#endif // TIMELINE_ITEMS_H
