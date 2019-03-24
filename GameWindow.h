#pragma once
#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QKeyEvent>
#include <QOpenGLTexture>
#include "MeshCollection.h"
#include "Player.h"
#include "Shader.h"
#include "Camera.h"
#include "Light.h"

class GameWindow : public QOpenGLWindow, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	~GameWindow();
	GameWindow() = default;
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;
	void teardownGL();

	MeshCollection *meshCollection;

protected:
	void SetTransformations();
	void MovePlayer();
	void keyPressEvent(QKeyEvent*) override;
	void keyReleaseEvent(QKeyEvent*) override;
	void mouseMoveEvent(QMouseEvent*) override;
	void wheelEvent(QWheelEvent*) override;
	void mousePressEvent(QMouseEvent*) override;
	void mouseReleaseEvent(QMouseEvent*) override;
private:
	Shader* shaderProgram;
	Player *player;
	Player* marker;

	Camera *camera;
	bool cameraXRotation = false;
	Light *light;

	std::map<char, bool> pressedKeys;
	QPointF mousePosition;
};
