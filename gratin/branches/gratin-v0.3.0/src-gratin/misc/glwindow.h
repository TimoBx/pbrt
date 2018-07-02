// This file is part of Gratin, a programmable Node-based System
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef GL_WINDOW_H
#define GL_WINDOW_H

#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>

#include "misc/vertexarrayObject.h"

class glwindow : public QWindow, public GlContext
{
    Q_OBJECT
public:
    explicit glwindow(QWindow *parent = 0);
    explicit glwindow(QScreen *screen);
    ~glwindow();

    virtual void render();

    virtual void initialize();

public slots:
    void renderLater();
    void renderNow();

protected:
    bool event(QEvent *event) Q_DECL_OVERRIDE;

    void exposeEvent(QExposeEvent *event) Q_DECL_OVERRIDE;

private:
    bool m_update_pending;
};

#endif // GL_WINDOW_H
