// This file is part of Gratin, a node-based compositing software 
// for 2D and 3D animations.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <qapplication.h>
#include <QString>
#include <stdio.h>
#include <stdlib.h>
#include "view/mainWindow.h"

using namespace std;

bool params   = false;

QString inFilename;

void getParams(int argc,char **argv) {

  for(int i=1;i<argc;i++) {    
    if(strcmp(argv[i],"-in")==0 && i<argc-1) {
      inFilename = QString(argv[i+1]);
    } 
  }
}

int main(int argc,char** argv) {
  QApplication application(argc,argv);

  application.setWindowIcon(QIcon(":/gratin-logo"));
 
  getParams(argc,argv);
  MainWindow window;

  window.setWindowTitle("Gratin");
  window.show();

  if(!inFilename.isEmpty())
    window.loadFile(inFilename);

  return application.exec();
}

