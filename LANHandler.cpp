#include "LANHandler.h"

LANHandler::LANHandler(int port)
{
	isServer = true;

	qDebug() << "Podlaczanie LAN jako server";
	server = new QTcpServer(this);

	connect(server, SIGNAL(newConnection()), this, SLOT(newClient()));
	qDebug() << "Server started listening: " << server->listen(QHostAddress::Any, port);
	id = 0;
}

LANHandler::LANHandler(std::string address, int port)
{
	isServer = false;

	socket = new QTcpSocket(this);
	socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
	socket->setReadBufferSize(156*2);

	socket->connectToHost(address.c_str(), port);
	socket->waitForConnected();

	char buffer[8];
	socket->waitForReadyRead();
	socket->read(buffer, 8);
	id = atoi(buffer);

	qDebug() << "Moje ID to: " << id;

	connect(socket, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
	connect(socket, SIGNAL(readyRead()), this, SLOT(clientReadyRead()));

	qDebug() << "Client connected to server: " << socket->waitForConnected();
}

LANHandler::~LANHandler()
{
}

void LANHandler::setPlayerAndEnemiesPointers(Player* player, std::vector<Player*>* enemies)
{
	this->player = player;
	this->enemies = enemies;
}

int LANHandler::GetID()
{
	if (isServer)
		return 0;
	else
		return id;
}

void LANHandler::sendToAllPlayers(SerializedPlayer serialized_player)
{
	QByteArray byte_array;
	QDataStream out(&byte_array, QIODevice::WriteOnly);
	out << serialized_player;

	for (int i = 0; i < players.size(); i++)
	{
		players[i]->write(byte_array);
		players[i]->flush();
	}
}

void LANHandler::SendPlayers()
{
	if (isServer)
	{
		for (auto enemy : *enemies)
		{
			SerializedPlayer serialized_player;
			enemy->Serialize(&serialized_player);
			sendToAllPlayers(serialized_player);
		}
		SerializedPlayer serialized_player;
		player->Serialize(&serialized_player);
		sendToAllPlayers(serialized_player);
	}
	else
	{
		QByteArray byte_array;
		QDataStream out(&byte_array, QIODevice::WriteOnly);
		SerializedPlayer serialized_player;
		player->Serialize(&serialized_player);
		out << serialized_player;

		socket->write(byte_array);
		socket->flush();
	}
}

void LANHandler::newClient()
{
	players.push_back(server->nextPendingConnection());
	players.back()->setSocketOption(QAbstractSocket::LowDelayOption, 1);
	players.back()->setReadBufferSize(156 * 2);

	connect(players.back(), SIGNAL(readyRead()), this, SLOT(serverGotData()));

	qDebug() << "NOWY KLIENT, nadaje mu ID = " << ++id;

	char buffer[8];
	itoa(id, buffer, 10);
	players.back()->write(buffer);
}

void LANHandler::serverGotData()
{
	for (auto socket : players)
		while(socket->bytesAvailable()>=156)
		{
			QByteArray bytes;
			bytes = socket->read(156);
			QDataStream out(&bytes, QIODevice::ReadOnly);
			SerializedPlayer serialized_player;

			out >> serialized_player;

			if (serialized_player.magic != 777)
				continue;
			if (serialized_player.HP <= 0)
				continue;

			bool found = false;
			for (auto enemy : *enemies)
				if (enemy->GetID() == serialized_player.id)
				{
					found = true;
					enemy->Deserialize(serialized_player);
					break;
				}
			if (found == false)
			{
				enemies->push_back(new Player(-1, MeshCollection::ModelTexture::Robot_Basic));
				enemies->back()->Deserialize(serialized_player);
				qDebug() << "NOWY GRACZ. ID = " << enemies->back()->GetID();
			}

			sendToAllPlayers(serialized_player);
		}
}

void LANHandler::clientDisconnected()
{}

void LANHandler::clientReadyRead()
{
	while (socket->bytesAvailable() >=8)
	{
		QByteArray bytes;
		bytes = socket->read(156);
		QDataStream out(&bytes, QIODevice::ReadOnly);
		SerializedPlayer serialized_player;

		out >> serialized_player;

		if (serialized_player.magic != 777)
			continue;

		if (serialized_player.id == player->GetID())
			continue;
		if (serialized_player.HP <= 0)
			continue;

		bool found = false;
		for (auto enemy : *enemies)
			if (enemy->GetID() == serialized_player.id)
			{
				found = true;
				enemy->Deserialize(serialized_player);
				break;
			}
		if (!found)
		{
			enemies->push_back(new Player(-1, MeshCollection::ModelTexture::Robot_Basic));
			enemies->back()->Deserialize(serialized_player);
			qDebug() << "NOWY GRACZ o ID" << enemies->back()->GetID();

		}
	}
}


QDataStream& operator<<(QDataStream& out, const SerializedPlayer& data)
{
	out << data.position;
	out << data.id;
	out << data.HP;
	out << data.actualAttack;
	out << data.attackingFramesLeft;
	out << data.color;
	out << static_cast<int>(data.currentAnimation);
	out << data.force;
	out << data.magic;
	out << static_cast<int>(data.previousAnimation);
	out << data.previousAnimationFramesLeft;
	out << data.rotation;
	out << data.simpleMovement;
	out << static_cast<int>(data.texture);
	return out;
}

QDataStream& operator>>(QDataStream& in, SerializedPlayer& data)
{
	in >> data.position;
	in >> data.id;
	in >> data.HP;
	in >> data.actualAttack;
	in >> data.attackingFramesLeft;
	in >> data.color;

	int temp;
	in >> temp;
	data.currentAnimation = static_cast<PlayerAnimations>(temp);
	in >> data.force;
	in >> data.magic;

	in >> temp;
	data.previousAnimation = static_cast<PlayerAnimations>(temp);
	in >> data.previousAnimationFramesLeft;
	in >> data.rotation;
	in >> data.simpleMovement;
	in >> temp;
	data.texture = static_cast<MeshCollection::ModelTexture>(temp);
	return in;
}