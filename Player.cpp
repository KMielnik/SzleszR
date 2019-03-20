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
	position -= QVector3D(.5f, 0, 0);
}

void Player::Draw()
{
	QMatrix4x4 transformations = initialTransformation;
	transformations.translate(position);
	transformations.rotate(rotation);

	meshCollection->Draw(MeshCollection::ModelType::Man, transformations);
}
