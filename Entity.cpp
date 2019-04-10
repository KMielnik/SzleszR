#include "Entity.h"
#include <QDebug>

Entity::Entity(MeshCollection::ModelType type, MeshCollection::ModelTexture texture)
{
	modelType = type;
	modelTexture = texture;

	meshCollection = MeshCollection::GetInstance();
	meshCollection->InitializeModel(modelType);
	meshCollection->InitializeTexture(modelTexture);

	initialTransformation.setToIdentity();
}

void Entity::Draw()
{
	QMatrix4x4 transformations = initialTransformation;
	transformations.translate(position);
	transformations.rotate(rotation);

	meshCollection->Draw(modelType, modelTexture,
		transformations);
}

void Entity::PerformLogicStep()
{
	position += rotation * QVector3D(simpleMovement.x(), 0, simpleMovement.y());
	simpleMovement /= 1.5f;
}

void Entity::Move(QVector3D direction)
{
	if (direction.length() == 0) return;

	simpleMovement = QVector2D(direction.x(),direction.z());
	simpleMovement.normalize();
	simpleMovement *= maxSpeed;
}

void Entity::Rotate(QQuaternion quaternion)
{
	rotation = quaternion * rotation;
}

QVector3D Entity::GetPosition()
{
	return position;
}

QQuaternion Entity::GetRotation()
{
	return rotation;
}
