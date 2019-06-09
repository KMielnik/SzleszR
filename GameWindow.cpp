#include "GameWindow.h"
#include <QDebug>
#include <corecrt_math_defines.h>

GameWindow::~GameWindow()
{
	makeCurrent();
	teardownGL();
	delete lanHandler;
}

GameWindow::GameWindow(LANHandler* lanHandler) : lanHandler(lanHandler)
{
}

void GameWindow::initializeGL()
{
	initializeOpenGLFunctions();
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);

	camera = new Camera();

	meshCollection = MeshCollection::GetInstance();
	meshCollection->Initialize(camera->getProjectionMatrixPointer(), camera->getCameraMatrixPointer(), &lights);


	if(lanHandler->GetID()%2)
		player = new Player(lanHandler->GetID(),MeshCollection::ModelTexture::Robot_Basic);
	else
		player = new Player(lanHandler->GetID(), MeshCollection::ModelTexture::Robot_Red);

	lanHandler->setPlayerAndEnemiesPointers(player, &enemies);


	player->SetPosition(QVector3D(1, 0, 1) * 2 * lanHandler->GetID());
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


	MovePlayer();
	if (player->IsAlive())
	{
		

		for (auto enemy : enemies)
		{
			player->CheckCollision(enemy);
			enemy->CheckCollision(player);
		}

		player->PerformLogicStep();
		for (auto enemy : enemies)
			enemy->PerformLogicStep();
	}

	for (int i = 0; i < enemies.size(); i++)
		if (!enemies[i]->IsAlive())
		{
			delete enemies[i];
			enemies.erase(enemies.begin()+i);
		}

	lanHandler->SendPlayers();

	SetTransformations();

	if(player->IsAlive())
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

	for (auto light : lights)
		light->setColor(QVector3D(0, 0, 0));

	for (auto sphere : spheres)
		sphere->setColor(QVector3D(0, 0, 0));

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