#include <QGuiApplication>
#include "GameWindow.h"
#include <iostream>

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);

	QSurfaceFormat format;
	format.setRenderableType(QSurfaceFormat::OpenGL);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setVersion(3, 3);
	bool isServer;
	std::cin >> isServer;

	LANHandler* lanHandler;
	if (isServer)
		lanHandler = new LANHandler(12345);
	else
		lanHandler = new LANHandler("localhost", 12345);

	GameWindow gameWindow(lanHandler);
	gameWindow.setFormat(format);
	gameWindow.resize(QSize(1500, 900));
	gameWindow.show();

	return app.exec();
}
