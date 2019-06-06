#include "GameWindow.h"
#include <QDebug>
#include <corecrt_math_defines.h>

GameWindow::~GameWindow()
{
	makeCurrent();
	teardownGL();
}

GameWindow::GameWindow(bool isServer) : isServer(isServer)
{
	qDebug() << "Podlaczenie lan";

	if(isServer)
	{
		server = new QTcpServer(this);

		connect(server, SIGNAL(newConnection()), this, SLOT(newClient()));
		qDebug() << "Server is listening: "<< server->listen(QHostAddress::Any, 12345);

		players.push_back(nullptr);
	}
	else
	{
		socket = new QTcpSocket(this);
		socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
		socket->setReadBufferSize(192);

		socket->connectToHost("localhost", 12345);
		socket->waitForConnected();
		char buffer[8];
		socket->waitForReadyRead();
		socket->read(buffer, 8);
		id = atoi(buffer);

		qDebug() << "Moje id to: " << id;

		connect(socket, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
		connect(socket, SIGNAL(readyRead()), this, SLOT(clientReadyRead()));
		connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(clientBytesWritten(qint64)));

		qDebug() << "Client connected to server: " << socket->waitForConnected(5000);
	}
}

void GameWindow::initializeGL()
{
	initializeOpenGLFunctions();
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);

	camera = new Camera();

	meshCollection = MeshCollection::GetInstance();
	meshCollection->Initialize(camera->getProjectionMatrixPointer(), camera->getCameraMatrixPointer(), &lights);

	
	player = new Player(id,MeshCollection::ModelTexture::Robot_Basic);
	player->SetPosition(QVector3D(1, 0, 1) * 2 * id);
	camera->SetPlayer(player);
	
	mousePosition = QPoint(size().width() / 2, size().height() / 2);

	terrain = new Terrain();
}

void GameWindow::resizeGL(int w, int h)
{
	camera->SetProjection(70.f, w, h, 0.01f, 100.f);
	mousePosition = QPoint(w/2, h/2);
	camera->Reset();
}

void GameWindow::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glShadeModel(GL_FLAT);

	SendPlayers();

	MovePlayer();

	for(auto enemy : enemies)
	{
		player->CheckCollision(enemy);
		enemy->CheckCollision(player);
	}

	player->PerformLogicStep();
	for(auto enemy : enemies)
		enemy->PerformLogicStep();
	
	SetTransformations();

	player->Draw();
	for(auto enemy : enemies)
		enemy->Draw();
	terrain->Draw();
	for(auto sphere : spheres)
		sphere->Draw();


	update();
}

void GameWindow::teardownGL()
{
	delete meshCollection;
}

void GameWindow::SetTransformations()
{
	std::vector<Player*> allPlayers;
	allPlayers.push_back(player);
	for (auto enemy : enemies)
		allPlayers.push_back(enemy);

	int lightNO = 0;

	//light enemies
	for(auto player : allPlayers)
	{
		if(lightNO>=lights.size())
		{
			spheres.push_back(new Sphere());
			lights.push_back(new Light(QVector3D(0, 2, 0), QVector3D(1, 1, 1)));
		}
		float theta = player->GetRotation().toEulerAngles().y();
		float offsetX = 1 * std::sin(theta * M_PI / 180)*-0.2;
		float offsetZ = 1 * std::cos(theta * M_PI / 180)*-0.2;
		lights[lightNO]->setPosition(player->GetPosition() + QVector3D(0 - offsetX, 4.5, 0 - offsetZ));
		lights[lightNO]->setColor(player->GetColor());

		spheres[lightNO]->SetPosition(player->GetPosition() + QVector3D(0 - offsetX, 4.5, 0 - offsetZ));
		spheres[lightNO]->setColor(player->GetColor());
		lightNO++;
	}

	camera->Move();
}

void GameWindow::MovePlayer()
{
	float speed = 1.f;
	float dx = 0;
	if (pressedKeys[Qt::Key_A]) dx += speed;
	if (pressedKeys[Qt::Key_D]) dx -= speed;

	float dz = 0;
	if (pressedKeys[Qt::Key_W]) dz += speed;
	if (pressedKeys[Qt::Key_S]) dz -= speed;

	player->Move(QVector3D(dx, 0, dz));
}

void GameWindow::SendPlayers()
{
	if (isServer)
		for (int i = 1; i < players.size(); i++)
		{
			for (int j = 0; j < players.size(); j++)
			{
				if (i == j) continue;
				SerializedPlayer serialized_player;
				if (player->GetID() == j)
					player->Serialize(&serialized_player);
				for (auto enemy : enemies)
					if (enemy->GetID() == j)
						enemy->Serialize(&serialized_player);

				QByteArray byte_array;
				QDataStream out(&byte_array, QIODevice::WriteOnly);
				out << serialized_player;

				players[i]->write(byte_array);
				players[i]->flush();
			}
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

void GameWindow::keyPressEvent(QKeyEvent* e)
{
	pressedKeys[e->key()] = true;
	if (e->key() == Qt::Key_Space)
		player->LongAttack();
}

void GameWindow::keyReleaseEvent(QKeyEvent* e)
{
	pressedKeys[e->key()] = false;
}

void GameWindow::mouseMoveEvent(QMouseEvent* e)
{
	if (cameraXRotation)
		camera->ChangeYaw(e->localPos().x() - mousePosition.x());
	else
		player->Rotate(QQuaternion::fromAxisAndAngle(0, 1, 0, (mousePosition.x() - e->localPos().x()) / 3));

	camera->ChangePitch(e->localPos().y() - mousePosition.y());

	mousePosition = e->localPos();
}

void GameWindow::wheelEvent(QWheelEvent* e)
{
	camera->ChangeZoom(e->delta());
}

void GameWindow::mousePressEvent(QMouseEvent* e)
{
	if (e->button() == Qt::MouseButton::LeftButton)
		player->ShortAttack();
	if (e->button() == Qt::MouseButton::RightButton)
		cameraXRotation = true;
}

void GameWindow::mouseReleaseEvent(QMouseEvent* e)
{
	if (e->button() == Qt::MouseButton::RightButton)
		cameraXRotation = false;
}

void GameWindow::mouseDoubleClickEvent(QMouseEvent* e)
{
	if (e->button() == Qt::MouseButton::RightButton)
		camera->ResetYaw();
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

void GameWindow::newClient()
{
	players.push_back(server->nextPendingConnection());
	players.back()->setSocketOption(QAbstractSocket::LowDelayOption, 1);
	connect(players.back(), SIGNAL(readyRead()), this, SLOT(serverGotData()));

	qDebug() << "NOWY KLIENT, nadaje mu ID = " << ++id;
	char buffer[8];
	itoa(id, buffer, 10);
	players.back()->write(buffer);
}

void GameWindow::serverGotData()
{
	for (auto socket : players)
	{
		if (socket == nullptr) continue;
		while (socket->bytesAvailable() >= 8)
		{
			QByteArray bytes;
			bytes = socket->read(156);
			QDataStream out(&bytes, QIODevice::ReadOnly);
			SerializedPlayer serialized_player;

			out >> serialized_player;

			if (serialized_player.magic != 777)
				continue;

			bool found = false;
			for (auto enemy : enemies)
				if (enemy->GetID() == serialized_player.id)
				{
					found = true;
					enemy->Deserialize(serialized_player);
					break;
				}
			if (found == false)
			{
				enemies.push_back(new Player(-1, MeshCollection::ModelTexture::Robot_Basic));
				enemies.back()->Deserialize(serialized_player);
				qDebug() << "NOWY CIOLEK" << enemies.back()->GetID();

			}
		}
	}
}

void GameWindow::clientDisconnected()
{
}

void GameWindow::clientBytesWritten(qint64 bytes)
{
	//qDebug() << "Udalo sie zapisac :" << bytes << "bajtow";
}

void GameWindow::clientReadyRead()
{
	while (socket->bytesAvailable() >= 8)
	{
		QByteArray bytes;
		bytes = socket->read(156);
		QDataStream out(&bytes, QIODevice::ReadOnly);
		SerializedPlayer serialized_player;

		out >> serialized_player;

		if (serialized_player.magic != 777)
			continue;

		bool found = false;
		for (auto enemy : enemies)
			if (enemy->GetID() == serialized_player.id)
			{
				found = true;
				enemy->Deserialize(serialized_player);
				break;
			}
		if (!found)
		{
			enemies.push_back(new Player(-1, MeshCollection::ModelTexture::Robot_Basic));
			enemies.back()->Deserialize(serialized_player);
			qDebug() << "NOWY CIOLEK" << enemies.back()->GetID();

		}
	}
}
