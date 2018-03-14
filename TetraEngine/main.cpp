#include "TetraEngine.h"
#include <QtWidgets/QApplication>
#include <SceneLoader.h>
#include <Scene.h>
#include <Camera.h>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	tetraRender::Camera cam;
	cam.getPos();

	TetraEngine w;
	w.show();
	return a.exec();
	
}
