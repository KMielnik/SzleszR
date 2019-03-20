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

	meshCollection = new MeshCollection(shaderProgram);

	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
	
	meshCollection->InitializeModel(MeshCollection::ModelType::Man);
}

void GameWindow::resizeGL(int w, int h)
{
}

void GameWindow::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);

	shaderProgram->bind();
	meshCollection->Draw(MeshCollection::ModelType::Man);
	shaderProgram->release();

	update();
}

void GameWindow::teardownGL()
{
	delete meshCollection;
	delete shaderProgram;
}
