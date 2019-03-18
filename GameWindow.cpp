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
}

void GameWindow::resizeGL(int w, int h)
{
}

void GameWindow::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void GameWindow::teardownGL()
{
	//
}
