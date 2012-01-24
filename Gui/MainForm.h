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

#ifndef MAINFORM_H
#define MAINFORM_H

#include <QtGui/QWidget>

class MainForm : public QWidget
{
  Q_OBJECT
public:
  MainForm(QWidget *parent = 0);

private slots:
  void on_angleSlider_valueChanged(int);
  void on_speedSlider_valueChanged(int);
  
};

#endif // MAINFORM_H
