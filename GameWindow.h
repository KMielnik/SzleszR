#pragma once
#include <QOpenGLWindow>
#include <QOpenGLFunctions>

class GameWindow : public QOpenGLWindow, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	~GameWindow();
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;
	void teardownGL();
};
