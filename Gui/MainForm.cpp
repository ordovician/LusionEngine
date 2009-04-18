#include "MainForm.h"

#include <QtCore/QFile>
#include <QtGui/QVBoxLayout>
#include <QtUiTools>

#include "Gui/RenderWidget.h"
#include "Lua/LuaEngine.h"

MainForm::MainForm(QWidget *parent)
     : QWidget (parent)
{
  QUiLoader loader;

  QFile file("Forms/MainForm.ui");
  file.open(QFile::ReadOnly);
  QWidget* form = loader.load(&file, this);  
  file.close();
  
  QMetaObject::connectSlotsByName(this);
  RenderWidget* render = new RenderWidget(this);
    
  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(render, 10);
  layout->addWidget(form, 1);
  setLayout(layout);

  setWindowTitle(tr("Lusion Engine"));
}

void MainForm::on_angleSlider_valueChanged(int value)
{
  luaSetNumberProperty("Engine.player.rotation", value);
}

void MainForm::on_speedSlider_valueChanged(int value)
{
  luaSetNumberProperty("Engine.player.speed", value);  
}
