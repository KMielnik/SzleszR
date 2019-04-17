#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <QVector3D>
#include "Player.h"



class Camera
{
public:
	Camera(Player* player, Shader* shader);

	void Move();
	void Reset();

	void ChangeZoom(float mouseWheelDelta);
	void ChangePitch(float mouseYAxis);
	void ChangeYaw(float mouseXAxis);
	void ResetYaw();
	void SetProjection(float FOV, float width, float height, float nearPlane, float farPlane);

private:
	QVector3D position;
	float pitch;
	float yaw;
	float distanceFromPlayer;

	float FOV;

	QMatrix4x4 projectionMatrix;
	QMatrix4x4 cameraMatrix;

	Player* player;
	Shader* shader;
};
