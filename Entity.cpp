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
	position += rotation * force;
	force /= 1.5f;

	position.setY(position.y() - gravitation);

	if (position.y() < 0)
		position.setY(0);
}

bool Entity::CheckCollision(Entity* entity)
{
	QVector3D v = entity->position - position;
	float d = v.length();

	if (d < (radius + entity->radius))
	{
		v.normalize();
		float energySum = force.length() + entity->force.length();
		force = rotation* -v * energySum / 2;
		entity->force = entity->rotation* v * energySum / 2;
		return true;
	}

	return false;
}

void Entity::Move(QVector3D direction)
{
	if (direction.length() == 0) return;

	simpleMovement = QVector2D(direction.x(),direction.z());
	simpleMovement.normalize();
	simpleMovement *= maxSpeed;

	force += QVector3D(simpleMovement.x(), 0, simpleMovement.y());
}

void Entity::SetPosition(QVector3D newPosition)
{
	position = newPosition;
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
