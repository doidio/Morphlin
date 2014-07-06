#include "Morphlin.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Morphlin w;
	w.showMaximized();
	return a.exec();
}
//QMessageBox::information(0, tr(""), tr(""));

//QString str;
//str.sprintf("%lf %lf %lf", center[0], center[1], center[2]);
//QMessageBox::information(0, tr(""), str);

//caller->Print(std::ostream(print_caller.txt));