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

void Player::ShortAttack()
{
	if (attackingFramesLeft <= 0)
	{
		attackingFramesLeft = shortAttackFrames;
		actualAttack = AttackTypes::Short;
	}
}

void Player::LongAttack()
{
	if (attackingFramesLeft <= 0)
	{
		attackingFramesLeft = longAttackFrames;
		actualAttack = AttackTypes::Long;
	}
}

void Player::SuperAttack()
{
}

Player::AttackTypes Player::isAttacking()
{
	switch(actualAttack)
	{
	case AttackTypes::NoAttack: 
		break;
	case AttackTypes::Short: 
		if (attackingFramesLeft < (shortAttackFrames * 0.15) || attackingFramesLeft >(shortAttackFrames * 0.85))
			return AttackTypes::NoAttack;
	case AttackTypes::Long: 
		if (attackingFramesLeft < (longAttackFrames * 0.25) || attackingFramesLeft >(longAttackFrames * 0.75))
			return AttackTypes::NoAttack;
	case AttackTypes::Super: break;
	}
	return actualAttack;
}

void Player::PerformLogicStep()
{
	Entity::PerformLogicStep();
	if(attackingFramesLeft>0)
		attackingFramesLeft--;
	if (attackingFramesLeft == 0)
		actualAttack = AttackTypes::NoAttack;

	if (HP < 0) HP = 0;
}

bool Player::CheckCollision(Entity* entity)
{
	bool innerCollisionOccured = Entity::CheckCollision(entity);

	bool attackCollisionOccured = false;

	//is another player?
	auto enemy = dynamic_cast<Player*>(entity);
	if(enemy!= nullptr)
	{
		if (enemy->isAttacking()==AttackTypes::Short)
		{
			QVector3D v = enemy->position - position;
			float d = v.length();

			if (d < (attackRadius + enemy->attackRadius))
			{
				HP -= 5;
				enemy->stopAttack();
				qDebug() << "HIT FOR: " << 5;
				attackCollisionOccured = true;
			}
		}

		if (enemy->isAttacking() == AttackTypes::Long)
		{
			QVector3D v = enemy->position - position;
			float d = v.length();

			if (d < (attackRadius + enemy->attackRadius))
			{
				HP -= 15;
				enemy->stopAttack();
				qDebug() << "HIT FOR: " << 15;
				attackCollisionOccured = true;
			}
		}
	}

	return innerCollisionOccured || attackCollisionOccured;
}

QVector3D Player::GetColor()
{
	if (isAttacking() == AttackTypes::Short)
	{
		return HP*QVector3D(0, 1, 0)/100;
	}
	else if (isAttacking() == AttackTypes::Long)
		return HP * QVector3D(0.5, 0, 0) / 100;
	else
		return HP*color/100;
}

void Player::stopAttack()
{
	switch(actualAttack)
	{
	case AttackTypes::Short:
		attackingFramesLeft = 40; 
		break;
	case AttackTypes::Long:
		attackingFramesLeft = 100;
		break;
	case AttackTypes::Super: break;
	default: ;
	}
	actualAttack = AttackTypes::NoAttack;
}
