#include "GameWindow.h"
#include <QDebug>

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

	light = new Light(QVector3D(1, 1, -1), QVector3D(1, 1, 1));
	
	player = new Player(MeshCollection::ModelTexture::Robot_Basic);

	camera = new Camera(player, shaderProgram);
	mousePosition = QPoint(size().width() / 2, size().height() / 2);
	
	marker = new Player(MeshCollection::ModelTexture::Robot_Red);
	marker->Move(QVector3D(0.5, 0, 0));	
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

	shaderProgram->Bind();
	SetTransformations();

	player->Draw();
	marker->Draw();

	shaderProgram->Release();

	update();
}

void GameWindow::teardownGL()
{
	delete meshCollection;
	delete shaderProgram;
}

void GameWindow::SetTransformations()
{
	camera->Move();
	shaderProgram->LoadLight(light);
}

void GameWindow::MovePlayer()
{
	float speed = 0.1f;
	float dx = 0;
	if (pressedKeys[Qt::Key_A]) dx += speed;
	if (pressedKeys[Qt::Key_D]) dx -= speed;

	float dz = 0;
	if (pressedKeys[Qt::Key_W]) dz += speed;
	if (pressedKeys[Qt::Key_S]) dz -= speed;

	float dy = 0;
	if (pressedKeys[Qt::Key_Q]) dy += speed;
	if (pressedKeys[Qt::Key_E]) dy -= speed;

	player->Move(QVector3D(-dx, 0, -dz));
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
	if (e->button() == Qt::MouseButton::RightButton)
		cameraXRotation = true;
}

void GameWindow::mouseReleaseEvent(QMouseEvent* e)
{
	if (e->button() == Qt::MouseButton::RightButton)
		cameraXRotation = false;
}
