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

	shaderProgram = new Shader();

	meshCollection = MeshCollection::GetInstance();
	meshCollection->Initialize(shaderProgram);

	for(int i=0;i<2;i++)
		lights.push_back(new Light( QVector3D(0, 2, 0), QVector3D(1, 1, 1)));

	player = new Player(MeshCollection::ModelTexture::Robot_Basic);

	camera = new Camera(player, shaderProgram);
	mousePosition = QPoint(size().width() / 2, size().height() / 2);
	
	marker = new Player(MeshCollection::ModelTexture::Robot_Red);
	marker->SetPosition(QVector3D(0, 0, 3));
	terrain = new Terrain();
	sphere = new Sphere(MeshCollection::ModelTexture::Sphere);
	sphere->SetPosition(QVector3D(1, 1, 1));
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

	player->CheckCollision(marker);
	marker->CheckCollision(player);

	player->PerformLogicStep();
	marker->PerformLogicStep();

	float theta = player->GetRotation().toEulerAngles().y();
	float offsetX = 2 * std::sin(theta * M_PI / 180);
	float offsetZ = 2 * std::cos(theta * M_PI / 180);

	lights[0]->setPosition(player->GetPosition() + QVector3D(0-offsetX, 7, 0-offsetZ));
	lights[0]->setColor(player->GetColor());
	sphere->SetPosition(player->GetPosition() + QVector3D(0 - offsetX, 7, 0 - offsetZ));

	theta = marker->GetRotation().toEulerAngles().y();
	offsetX = 2 * std::sin(theta * M_PI / 180);
	offsetZ = 2 * std::cos(theta * M_PI / 180);
	lights[1]->setPosition(marker->GetPosition() + QVector3D(0-offsetX, 7, 0-offsetZ));
	lights[1]->setColor(marker->GetColor());


	
	SetTransformations();

	player->Draw();
	marker->Draw();
	terrain->Draw();
	sphere->Draw();


	update();
}

void GameWindow::teardownGL()
{
	delete meshCollection;
	delete shaderProgram;
}

void GameWindow::SetTransformations()
{
	shaderProgram->Bind();

	camera->Move();
	shaderProgram->LoadLights(lights);

	shaderProgram->Release();
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
