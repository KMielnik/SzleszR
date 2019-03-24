#pragma once
#include "IEntity.h"
#include <QVector3D>
#include <QQuaternion>
#include "MeshCollection.h"

class Player : IEntity
{
public:
	~Player() override;
	Player(MeshCollection::ModelTexture modelTexture);
	void Draw() override;
	void Move(QVector3D dPosition);
	void Rotate(QQuaternion quaternion);

	QVector3D GetPosition();
	QQuaternion GetRotation();

private:
	QMatrix4x4 initialTransformation;
	QVector3D position;
	QQuaternion rotation;
	MeshCollection* meshCollection;

	MeshCollection::ModelTexture playerModelTexture;
};
