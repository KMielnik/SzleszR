#pragma once
#include <QOpenGLWindow>
#include <QCloseEvent>
#include <QOpenGLFunctions>
#include <QKeyEvent>
#include <QOpenGLTexture>
#include "MeshCollection.h"
#include "Player.h"
#include "Shader.h"
#include "Camera.h"
#include "Light.h"
#include "Terrain.h"
#include "Sphere.h"
#include "LANHandler.h"

#define MAX_LIGHTS 10

class GameWindow : public QOpenGLWindow, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	~GameWindow();
	GameWindow(LANHandler* lanHandler);
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
	void mouseDoubleClickEvent(QMouseEvent*) override;
private:
	LANHandler* lanHandler;
	Shader* shaderProgram;
	Player *player;
	std::vector<Player *> enemies;
	Terrain* terrain;
	std::vector<Sphere*> spheres;

	Camera *camera;
	bool cameraXRotation = false;
	std::vector<Light *> lights;

	std::map<char, bool> pressedKeys;
	QPointF mousePosition;


	bool previousPlayers = 0;
};
