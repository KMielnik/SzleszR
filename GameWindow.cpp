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

	shaderProgram = new QOpenGLShaderProgram();
	shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, "Resources/simple.vert");
	shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, "Resources/simple.frag");

	shaderProgram->link();
	shaderProgram->bind();

	meshCollection = MeshCollection::GetInstance();
	meshCollection->Initialize(shaderProgram);

	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
	
	player = new Player();
}

void GameWindow::resizeGL(int w, int h)
{
}

void GameWindow::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);

	shaderProgram->bind();
	player->Draw();
	shaderProgram->release();

	update();
}

void GameWindow::teardownGL()
{
	delete meshCollection;
	delete shaderProgram;
}
