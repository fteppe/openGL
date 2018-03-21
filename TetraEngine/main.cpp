#include <SceneLoader.h>
#include <Scene.h>
#include <Camera.h>

#include "TetraEngine.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	TetraEngine w;
	tetraRender::Camera cam(600.0f, 800.0f, 0.75f);
	//tetraRender::Scene scene(cam);
	//scene.load("obj/scene.json");
	//scene.renderScene();

	
	w.show();
	return a.exec();
	
}
