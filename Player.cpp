#include "Player.h"
#include <QDebug>

Player::~Player()
{
}

Player::Player(MeshCollection::ModelTexture modelTexture)
{
	meshCollection = MeshCollection::GetInstance();
	meshCollection->InitializeModel(MeshCollection::ModelType::Robot);
	meshCollection->InitializeTexture(modelTexture);

	playerModelTexture = modelTexture;

	initialTransformation.setToIdentity();
}

void Player::Draw()
{
	QMatrix4x4 transformations = initialTransformation;
	transformations.translate(position);
	transformations.rotate(rotation);

	meshCollection->Draw(MeshCollection::ModelType::Robot, playerModelTexture,
		transformations);
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
