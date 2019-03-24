#pragma once
#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QKeyEvent>
#include <QOpenGLTexture>
#include "MeshCollection.h"
#include "Player.h"

class GameWindow : public QOpenGLWindow, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	~GameWindow();
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;
	void teardownGL();

	MeshCollection *meshCollection;

protected:
	void SetTransformations();
	void MoveCamera();
	void keyPressEvent(QKeyEvent*) override;
	void keyReleaseEvent(QKeyEvent*) override;
	void mouseMoveEvent(QMouseEvent*) override;
	void wheelEvent(QWheelEvent*) override;
private:
	QOpenGLShaderProgram* shaderProgram;
	Player *player;
	Player* marker;

	QMatrix4x4 projectionMatrix;
	QMatrix4x4 cameraMatrix;

	int projectionMatrixLoc = 0;
	int cameraMatrixLoc = 0;

	std::map<char, bool> pressedKeys;
	QPointF mousePosition;
	float distance = 1.5f;
};
