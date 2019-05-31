#include "Player.h"
#include <QDebug>

Player::~Player()
{
}

Player::Player(MeshCollection::ModelTexture modelTexture) : Entity(MeshCollection::ModelType::Robot,modelTexture)
{
	switch(modelTexture)
	{
	case MeshCollection::ModelTexture::Robot_Basic:
		color = QVector3D(1, 0, 1);
		break;
	case MeshCollection::ModelTexture::Robot_Red:
		color = QVector3D(0, 0, 1);
		break;
	}
}

void Player::Attack()
{
	if (attackingFramesLeft <= 0)
		attackingFramesLeft = 30;
}

bool Player::isAttacking()
{
	return attackingFramesLeft > 0;
}

void Player::PerformLogicStep()
{
	Entity::PerformLogicStep();
	if(attackingFramesLeft>0)
		attackingFramesLeft--;
	if (HP < 0) HP = 0;
	if (HP == 0)
		rotation = rotation*QQuaternion::fromAxisAndAngle(0, 1, 0, 3);
}

bool Player::CheckCollision(Entity* entity)
{
	bool innerCollisionOccured = Entity::CheckCollision(entity);

	bool attackCollisionOccured = false;

	//is another player?
	auto enemy = dynamic_cast<Player*>(entity);
	if(enemy!= nullptr)
	{
		if (enemy->isAttacking())
		{
			QVector3D v = enemy->position - position;
			float d = v.length();

			if (d < (attackRadius + enemy->attackRadius))
			{
				HP--;
				attackCollisionOccured = true;
			}
		}
	}

	return innerCollisionOccured || attackCollisionOccured;
}

QVector3D Player::GetColor()
{
	if (isAttacking())
	{
		return HP*QVector3D(0, 1, 0)/100;
	}
	else
		return HP*color/100;
}
