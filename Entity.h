#pragma once
#include <QVector3D>
#include "MeshCollection.h"

class Entity
{
public:
	Entity(MeshCollection::ModelType type, MeshCollection::ModelTexture texture);
	virtual ~Entity() = default;
	virtual void Draw();

	virtual void PerformLogicStep();

	virtual void Move(QVector3D direction);
	virtual void Rotate(QQuaternion quaternion);

	virtual QVector3D GetPosition();
	virtual QQuaternion GetRotation();

protected:
	MeshCollection::ModelTexture modelTexture;
	MeshCollection::ModelType modelType;

	QMatrix4x4 initialTransformation;
	QVector3D position;
	float gravitation;
	float maxSpeed = 0.3;
	QVector2D simpleMovement;
	QQuaternion rotation;
	MeshCollection* meshCollection;
};

