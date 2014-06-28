#include "morphlin.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Morphlin w;
	w.show();
	return a.exec();
}
