#include "GameWindow.h"
#include <QDebug>
#include <corecrt_math_defines.h>

GameWindow::~GameWindow()
{
	makeCurrent();
	teardownGL();
}

void GameWindow::initializeGL()
{
	initializeOpenGLFunctions();
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);

	camera = new Camera();

	meshCollection = MeshCollection::GetInstance();
	meshCollection->Initialize(camera->getProjectionMatrixPointer(), camera->getCameraMatrixPointer(), &lights);

	

	player = new Player(MeshCollection::ModelTexture::Robot_Basic);
	camera->SetPlayer(player);
	
	mousePosition = QPoint(size().width() / 2, size().height() / 2);
	
	enemies.push_back(new Player(MeshCollection::ModelTexture::Robot_Red));
	enemies.push_back(new Player(MeshCollection::ModelTexture::Robot_Red));
	enemies.push_back(new Player(MeshCollection::ModelTexture::Robot_Red));
	enemies[0]->SetPosition(QVector3D(0, 0, 3));
	enemies[1]->SetPosition(QVector3D(4, 0, 3));
	enemies[2]->SetPosition(QVector3D(-4, 0, 3));
	
	terrain = new Terrain();

	
	spheres.push_back(new Sphere());
	for(auto enemy :enemies)
	{
		spheres.push_back(new Sphere());
	}
	for (auto sphere : spheres)
		lights.push_back(new Light(QVector3D(0, 2, 0), QVector3D(1, 1, 1)));
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
	int lightNO = 0;
	//light player
	float theta = player->GetRotation().toEulerAngles().y();
	float offsetX = 1 * std::sin(theta * M_PI / 180);
	float offsetZ = 1 * std::cos(theta * M_PI / 180);
	lights[lightNO]->setPosition(player->GetPosition() + QVector3D(0 - offsetX, 6, 0 - offsetZ));
	lights[lightNO]->setColor(player->GetColor());

	spheres[lightNO]->SetPosition(player->GetPosition() + QVector3D(0 - offsetX, 6, 0 - offsetZ));
	spheres[lightNO]->setColor(player->GetColor());

	//light enemies
	for(auto enemy : enemies)
	{
		lightNO++;
		float theta = enemy->GetRotation().toEulerAngles().y();
		float offsetX = 1 * std::sin(theta * M_PI / 180);
		float offsetZ = 1 * std::cos(theta * M_PI / 180);
		lights[lightNO]->setPosition(enemy->GetPosition() + QVector3D(0 - offsetX, 6, 0 - offsetZ));
		lights[lightNO]->setColor(enemy->GetColor());

		spheres[lightNO]->SetPosition(enemy->GetPosition() + QVector3D(0 - offsetX, 6, 0 - offsetZ));
		spheres[lightNO]->setColor(enemy->GetColor());
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
		player->Attack();
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
