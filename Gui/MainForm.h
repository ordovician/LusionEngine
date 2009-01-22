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
