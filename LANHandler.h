#pragma once
#include <QTcpServer>
#include <QTcpSocket>
#include "Player.h"

QDataStream& operator <<(QDataStream& out, const SerializedPlayer& data);
QDataStream& operator >>(QDataStream& in, SerializedPlayer& data);

class LANHandler : QObject
{
	Q_OBJECT

public:
	LANHandler(int port);
	LANHandler(std::string address,int port);
	~LANHandler();
	void setPlayerAndEnemiesPointers(Player* player, std::vector<Player*>* enemies);

	int GetID();
	void SendPlayers();

private:
	bool isServer;
	int id;

	void sendToAllPlayers(SerializedPlayer serialized_player);

	std::vector<Player*> *enemies;
	Player* player;

	QTcpServer* server;
	std::vector<QTcpSocket*> players;

	QTcpSocket* socket;

public slots:
	void newClient();
	void serverGotData();

	void clientDisconnected();
	void clientReadyRead();
};
