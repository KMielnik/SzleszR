#pragma once
#include "IEntity.h"
#include <QVector3D>
#include <QQuaternion>
#include "MeshCollection.h"

class Player : IEntity
{
public:
	~Player() override;
	Player();
	void Draw() override;

private:
	QMatrix4x4 initialTransformation;
	QVector3D position;
	QQuaternion rotation;
	MeshCollection* meshCollection;
};
