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
	virtual bool CheckCollision(Entity *entity);

	virtual void Move(QVector3D direction);
	virtual void SetPosition(QVector3D newPosition);
	virtual void Rotate(QQuaternion quaternion);

	virtual QVector3D GetPosition();
	virtual QQuaternion GetRotation();

protected:
	MeshCollection::ModelTexture modelTexture;
	MeshCollection::ModelType modelType;

	QMatrix4x4 initialTransformation;
	QVector3D position;
	float innerRadius = 0.5f;
	float gravitation = 0.5f;
	float maxSpeed = 0.08f;
	QVector2D simpleMovement;
	QVector3D force;
	QQuaternion rotation;
	MeshCollection* meshCollection;
};

