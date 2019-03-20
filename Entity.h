#pragma once
#include "IEntity.h"
#include <QVector3D>
#include <QQuaternion>

class Player : IEntity
{
public:
	~Player() override;
	void Draw() override;

private:
	QVector3D position;
	QQuaternion rotation;
};
