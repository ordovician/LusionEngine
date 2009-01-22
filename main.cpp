/*
 *  main.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 19.12.06.
 *  Copyright 2006 Translusion. All rights reserved.
 *
 */

#include "Engine.h"

#include <QtGui/QApplication>

#include <Gui/MainForm.h>

#include <Core/AutoreleasePool.hpp>

int main(int argc, char *argv[])
{  
  AutoreleasePool::begin();
    QApplication a(argc, argv);
  
    QWidget* w = new MainForm;
    w->resize(640, 480);
    a.setActiveWindow( w );
    w->show();
    
    int result = a.exec();
    delete w;
    engineCleanup();
  AutoreleasePool::end();
          
  return result;
}
