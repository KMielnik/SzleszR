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

	std::cout << "Witaj w grze /R." << std::endl;
	std::cout << "Jest to gra multiplayer, w ktorej musisz pokonac pozostalych graczy." << std::endl;
	std::cout << "Poruszanie: WASD." << std::endl;
	std::cout << "Kamera: Mysz." << std::endl;
	std::cout << "Lekki atak: LPM" << std::endl;
	std::cout << "Ciezki atak: Spacja" << std::endl;
	std::cout << "Block(zmienjsza otrzymana obrazenia): Shift" << std::endl;
	std::cout << "Kolor i moc twojego swiatla sygnalizuje pozostale zycie." << std::endl;
	std::cout << "Gdy przegrasz, pozostanie po tobie jedynie bezbarwna kula." << std::endl;
	std::cout << std::endl << std::endl;

	std::cout << "Czy chcesz być serwerem? (1/0) - (prawda/falsz): ";
	bool isServer;
	std::cin >> isServer;
	std::cout << std::endl << std::endl;

	std::string address;
	if(!isServer)
	{
		std::cout << "Podaj adres: " << std::endl;
		std::cin >> address;
		std::cout << std::endl << std::endl;
	}

	int port;
	std::cout << "Podaj port: ";
	std::cin >> port;
	std::cout << std::endl << std::endl;


	LANHandler* lanHandler;
	if (isServer)
		lanHandler = new LANHandler(port);
	else
		lanHandler = new LANHandler(address, port);

	GameWindow gameWindow(lanHandler);
	gameWindow.setFormat(format);
	gameWindow.resize(QSize(1500, 900));
	gameWindow.show();

	return app.exec();
}
