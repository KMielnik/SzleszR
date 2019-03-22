#include "Player.h"
#include <QDebug>

Player::~Player()
{
}

Player::Player()
{
	meshCollection = MeshCollection::GetInstance();
	meshCollection->InitializeModel(MeshCollection::ModelType::Man);

	initialTransformation.setToIdentity();
}

void Player::Draw()
{
	QMatrix4x4 transformations = initialTransformation;
	transformations.translate(position);
	transformations.rotate(rotation);

	meshCollection->Draw(MeshCollection::ModelType::Man, transformations);
}

void Player::Move(QVector3D dPosition)
{
	position -= rotation*dPosition;
}

void Player::Rotate(QQuaternion quaternion)
{
	rotation=quaternion*rotation;
}

QVector3D Player::GetPosition()
{
	return position;
}

QQuaternion Player::GetRotation()
{
	return rotation;
}
