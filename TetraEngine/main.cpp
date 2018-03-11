#include "TetraEngine.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	TetraEngine w;
	w.show();
	return a.exec();
}
