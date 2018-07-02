// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef GENERIC_CUSTOM_WIDGET
#define GENERIC_CUSTOM_WIDGET

#include <QWidget>
#include <QDialog>
#include <QToolButton>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QTabWidget>
#include <QVBoxLayout>

#include "view/nodeWidget.h"
#include "view/widgetParams.h"
#include "core/nodeTexture2D.h"

class GenericCustomNode;
class GenericCustomWidget;

class GenericCustomIOItemButtons : public QWidget {
Q_OBJECT

  public:
  GenericCustomIOItemButtons();

  inline QToolButton *up()   const {return _up;}
  inline QToolButton *down() const {return _down;}
  inline QToolButton *del()  const {return _del;}

  private:
  QToolButton *_up;
  QToolButton *_down;
  QToolButton *_del;
};

class GenericCustomIOItem : public QWidget {
Q_OBJECT
  
  public:
  GenericCustomIOItem(unsigned int id=0,const QString &name=QString());

  inline void setId(unsigned int id) {_id=id;_number->setText(QString::number(_id));} 
  inline unsigned int id() const {return _id;}
  inline const QString name() const {return _name->text();}
  inline void setName(const QString &name) {_name->setText(name);}

 signals:
  void upClicked(GenericCustomIOItem *);
  void downClicked(GenericCustomIOItem *);
  void delClicked(GenericCustomIOItem *);

  private slots:
  void needUpSignal();
  void needDownSignal();
  void needDelSignal();

 private:
  unsigned int            _id;
  QLabel                 *_number;
  QLineEdit              *_name;
  GenericCustomIOItemButtons *_buttons;
};

class GenericCustomParamItem : public QWidget {
Q_OBJECT
  
  public:
  GenericCustomParamItem(unsigned int id=0,const QString &name=QString(),int type=0,bool kf=false,float minVal=0.0f,float maxVal=1.0f,float defaultVal=0.0f);

  inline void setId(unsigned int id) {_id=id;} 
  inline void setName(const QString &name) {_name->setText(name);}

  inline unsigned int  id()   const {return _id;                     }
  inline const QString name() const {return _name->text();           }

  inline bool  isKF  () const {return _kf->checkState()==Qt::Checked;}
  inline int   type  () const {return _type->currentIndex();         }
  inline float minVal() const {return _minEdit->text().toFloat();    }
  inline float maxVal() const {return _maxEdit->text().toFloat();    }
  inline float val   () const {return _valEdit->text().toFloat();    }

  static const QStringList TYPES;

 signals:
  void delClicked(GenericCustomParamItem *);

  private slots:
  void needDelSignal();

 private:
  unsigned int  _id;
  QLineEdit    *_name;
  QLineEdit    *_minEdit;
  QLineEdit    *_maxEdit;
  QLineEdit    *_valEdit;
  QLabel       *_minLabel;
  QLabel       *_maxLabel;
  QLabel       *_valLabel;
  QComboBox    *_type;
  QCheckBox    *_kf;
  QToolButton  *_del;
};

class GenericCustomWidgetSetting : public QDialog {
  Q_OBJECT
    
    public:
  GenericCustomWidgetSetting(GenericCustomWidget *nodeWidget,bool needMouseWidget=true);
  ~GenericCustomWidgetSetting();

  std::vector<GenericCustomIOItem *> inputItems() const;
  std::vector<GenericCustomIOItem *> outputItems() const;
  std::vector<GenericCustomParamItem *> paramItems() const;
  const Vector2f tsize(bool *ok=0) const;
  const Vector2f toffset(bool *ok=0) const;
  bool useMouse() const;

  private slots:
  // input slots
  void inputItemUp(GenericCustomIOItem *);
  void inputItemDown(GenericCustomIOItem *);
  void inputItemDel(GenericCustomIOItem *);
  void addInputClicked();

  // output slots
  void outputItemUp(GenericCustomIOItem *);
  void outputItemDown(GenericCustomIOItem *);
  void outputItemDel(GenericCustomIOItem *);
  void addOutputClicked();

  // param slots
  void paramItemDel(GenericCustomParamItem *);
  void addParamClicked();

  // buttons slots 
  void confirmClicked();
  void cancelClicked();

  protected:
  GenericCustomWidget *_nodeWidget;

  // input functions
  void buildInputLayout();
  void cleanInputLayout();
  void initInputItems();
  void cleanInputItems();

  // output functions
  void buildOutputLayout();
  void cleanOutputLayout();
  void initOutputItems();
  void cleanOutputItems();

  // param functions
  void buildParamLayout();
  void cleanParamLayout();
  void initParamItems();
  void cleanParamItems();

  // warning widgets 
  void showErrorMessage(const QString &msg);
  bool checkParams();

  // inputs 
  QGroupBox       *_inputBox;
  QPushButton     *_addInput;
  QVBoxLayout     *_inputLayout;
  std::vector<GenericCustomIOItem *> _inputItems;

  // outputs  
  QGroupBox       *_outputBox;
  QPushButton     *_addOutput;
  QVBoxLayout     *_outputLayout;
  std::vector<GenericCustomIOItem *> _outputItems;

  // params
  QGroupBox       *_paramBox;
  QPushButton     *_addParam;
  QVBoxLayout     *_paramLayout;
  std::vector<GenericCustomParamItem *> _paramItems;

  // title
  QLabel *_title;
  
  // special settings 
  QGroupBox       *_specialBox;
  QLineEdit       *_xSize;
  QLineEdit       *_ySize;
  QLineEdit       *_xOffset;
  QLineEdit       *_yOffset;
  QCheckBox       *_useMouse;

  // buttons
  QDialogButtonBox *_buttons;
};

class GenericCustomWidget : public NodeWidget {
  Q_OBJECT

    public:
  GenericCustomWidget(GenericCustomNode *node);

  const std::vector<QString> inputNames();
  const std::vector<QString> outputNames();

  inline std::vector<SliderFloat   *> floatSliders() {return _floatSliders;}
  inline std::vector<SliderInt     *> intSliders() {return _intSliders;}

  virtual void addUniformsToProg(GPUProgram *p);
  virtual void sendUniformsToProg(GPUProgram *p);

  bool nodePartiallyConnected() const;
  bool useMouse() const {return _mouseLabel;}
  Vector2f tsize() const;
  Vector2f toffset() const;

  // specific for save/created widgets (called by the node itself, before widgets have been created)
  bool saveWidgets(SceneSaver  *saver );
  bool loadWidgets(SceneLoader *loader);

  // called by nodeWidget after widgets have been created 
  bool loadUserDefinedWidgets(SceneLoader *);

  // mouse position 
  inline void setMousePosition(const Vector2f &val);

  // layout containing the user defined parameters
  inline QVBoxLayout *userLayout() {return _userLayout;}

 protected:
  void cleanLayout();
  void initLayout();
  void cleanWidgets();
  void initWidgets(const GenericCustomWidgetSetting &settings);
  void initFromSettings(const GenericCustomWidgetSetting &settings);

  QString generateHead(const QString &b,bool addOutputLocations=true);
  virtual const QStringList getUniformNames();

  GenericCustomNode *_customNode;

  std::vector<SliderFloat   *> _floatSliders;
  std::vector<SliderInt     *> _intSliders;
  QVBoxLayout   *_userLayout;
  KFLabel2Float *_mouseLabel;
};

inline void GenericCustomWidget::setMousePosition(const Vector2f &val) {
  if(useMouse()) {
    _mouseLabel->setVal(val);
  }
}

class GenericCustomNode : public NodeTexture2D {
 public:
  GenericCustomNode(PbGraph *parent=NULL,NodeHandle *handle=NULL,unsigned int nbInputs=1,unsigned int nbOutputs=1);
  virtual ~GenericCustomNode() {}

  inline unsigned int nbInputs()  const {return _nbInputs;}
  inline unsigned int nbOutputs() const {return _nbOutputs;}
  inline bool outputSizeEqualInputSize() const {return false;}
  virtual void initOutputData();

  virtual bool save(SceneSaver  *saver);
  virtual bool load(SceneLoader *loader);

  virtual const QStringList inputDesc();
  virtual const QStringList outputDesc();

 public:
  // specific functions 
  inline Vector2f tsize()   const {return _tsize;}
  inline Vector2f toffset() const {return _toffset;}

  virtual void setIONumbers(unsigned int nbInputs,unsigned int nbOutputs);
  virtual void setIONames(const QStringList &inames,const QStringList &onames);
  virtual void setImgSize(const Vector2f &tsize,const Vector2f &toffset);

 protected:
  unsigned int _nbInputs;
  unsigned int _nbOutputs;
  Vector2f     _tsize;
  Vector2f     _toffset;
  QStringList  _inames;
  QStringList  _onames;
};



#endif // GENERIC_CUSTOM_WIDGET
