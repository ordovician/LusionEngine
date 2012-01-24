/*
	LusionEngine- 2D game engine written in C++ with Lua interface.
	Copyright (C) 2006  Erik Engheim

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

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
