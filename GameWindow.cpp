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


	shaderProgram = new QOpenGLShaderProgram();
	shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, "Resources/simple.vert");
	shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, "Resources/simple.frag");

	shaderProgram->link();
	shaderProgram->bind();

	meshCollection = MeshCollection::GetInstance();
	meshCollection->Initialize(shaderProgram);

	projectionMatrixLoc = shaderProgram->uniformLocation("projectionMatrix");
	cameraMatrixLoc = shaderProgram->uniformLocation("cameraMatrix");

	cameraMatrix.setToIdentity();
	cameraMatrix.translate(0, -1.0f, -1);
	cameraMatrix.rotate(30, 1, 0, 0);
	cameraMatrix.lookAt(QVector3D(0, 0, 0), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
	
	player = new Player();
	
	marker = new Player();
	marker->Move(QVector3D(0.5, 0, 0));
	texture = new QOpenGLTexture(QImage("Resources/wire.jpg").mirrored());
	texture->setAutoMipMapGenerationEnabled(true);
	texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
	texture->setMagnificationFilter(QOpenGLTexture::Linear);
	
}

void GameWindow::resizeGL(int w, int h)
{
	projectionMatrix.setToIdentity();
	projectionMatrix.perspective(70.f, static_cast<float>(w) / h, 0.01f, 100.f);
}

void GameWindow::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glShadeModel(GL_FLAT);

	
	MoveCamera();
	cameraMatrix.setToIdentity();

	cameraMatrix.lookAt(player->GetPosition()+player->GetRotation()*QVector3D(0,distance,-1), player->GetPosition() + QVector3D(0,0.8,0), QVector3D(0,1,0));
	

	shaderProgram->bind();
	texture->bind();
	SetTransformations();


	player->Draw();
	marker->Draw();
	shaderProgram->release();

	update();
}

void GameWindow::teardownGL()
{
	delete meshCollection;
	delete shaderProgram;
}

void GameWindow::SetTransformations()
{
	shaderProgram->setUniformValue(projectionMatrixLoc, projectionMatrix);
	shaderProgram->setUniformValue(cameraMatrixLoc, cameraMatrix);
}

void GameWindow::MoveCamera()
{
	float speed = 0.05f;
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
	//

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
	if (e->globalPos() != QPoint(width() / 2, height() / 2))
	{
		player->Rotate(QQuaternion::fromAxisAndAngle(0, 1, 0, (mousePosition.x() - e->globalPos().x()) / 3));
		mousePosition = e->globalPos();
	}
}

void GameWindow::wheelEvent(QWheelEvent* e)
{
	distance -= (float)e->delta()/500;
	qDebug() << distance;
}
