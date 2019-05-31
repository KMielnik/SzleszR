#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <QVector3D>
#include "Player.h"



class Camera
{
public:
	Camera();

	void Move();
	void SetPlayer(Player* player);
	void LoadCamera(Shader *shader);
	void Reset();

	void ChangeZoom(float mouseWheelDelta);
	void ChangePitch(float mouseYAxis);
	void ChangeYaw(float mouseXAxis);
	void ResetYaw();
	void SetProjection(float FOV, float width, float height, float nearPlane, float farPlane);

	QMatrix4x4* getProjectionMatrixPointer();
	QMatrix4x4* getCameraMatrixPointer();

private:
	QVector3D position;
	float pitch;
	float yaw;
	float distanceFromPlayer;

	float FOV;

	QMatrix4x4 projectionMatrix;
	QMatrix4x4 cameraMatrix;

	Player* player;
};
