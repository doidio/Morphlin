#ifndef MORPHLIN_H
#define MORPHLIN_H

#include <QtWidgets/QMainWindow>
#include "ui_Morphlin.h"

class Morphlin : public QMainWindow
{
	Q_OBJECT

public:
	Morphlin(QWidget *parent = 0);
	~Morphlin();

private:
	Ui::MorphlinClass ui;
};

#endif // MORPHLIN_H
