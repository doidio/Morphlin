#ifndef CUSTOMQVTKWIDGET_H
#define CUSTOMQVTKWIDGET_H

#include <QVTKWidget.h>

class CustomQVTKWidget : public QVTKWidget
{
	Q_OBJECT

public:
	CustomQVTKWidget(QWidget *);
	~CustomQVTKWidget();

private:
	
};

#endif // CUSTOMQVTKWIDGET_H
