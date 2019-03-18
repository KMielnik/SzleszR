#include <QGuiApplication>
#include "GameWindow.h"

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);

	QSurfaceFormat format;
	format.setRenderableType(QSurfaceFormat::OpenGL);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setVersion(3, 3);

	GameWindow gameWindow;
	gameWindow.setFormat(format);
	gameWindow.resize(QSize(800, 600));
	gameWindow.show();

	return app.exec();
}
