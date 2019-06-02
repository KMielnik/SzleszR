﻿#include "Player.h"
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
		changeAnimation(PlayerAnimations::Attacking_Short);
	}
}

void Player::LongAttack()
{
	if (attackingFramesLeft <= 0)
	{
		attackingFramesLeft = longAttackFrames;
		actualAttack = AttackTypes::Long;
		changeAnimation(PlayerAnimations::Attacking_Long);
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
		if (attackingFramesLeft < (longAttackFrames * 0.2) || attackingFramesLeft >(longAttackFrames * 0.8))
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

	if (actualAttack == AttackTypes::Short)
		if (abs(attackingFramesLeft - (shortAttackFrames * 0.15)) < 1)
			changeAnimation(PlayerAnimations::Cooldown);

	if (actualAttack == AttackTypes::Long)
		if (abs(attackingFramesLeft - (longAttackFrames * 0.2)) < 1)
			changeAnimation(PlayerAnimations::Cooldown);

	if (actualAttack == AttackTypes::Short)
		if (abs(attackingFramesLeft - (shortAttackFrames * 0.85)) < 1)
			changeAnimation(PlayerAnimations::Windup);

	if (actualAttack == AttackTypes::Long)
		if (abs(attackingFramesLeft - (longAttackFrames * 0.8)) < 1)
			changeAnimation(PlayerAnimations::Windup);

	if (attackingFramesLeft == 0)
	{
		actualAttack = AttackTypes::NoAttack;
		changeAnimation(PlayerAnimations::Ready);
	}

	if (HP < 0) HP = 0;

	if (previousAnimationFramesLeft > 0)
		previousAnimationFramesLeft--;
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

			if (d < ((attackRadius + enemy->attackRadius)*1.5))
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
		return HP * QVector3D(0, 1, 0) / 100;
	if (isAttacking() == AttackTypes::Long)
		return HP * QVector3D(0.5, 0, 0) / 100;
	return HP*color/100;

}

void Player::Draw()
{
	QMatrix4x4 transformations = initialTransformation;
	transformations.translate(position);
	transformations.rotate(rotation);

	meshCollection->Draw(modelType, modelTexture,
		transformations,currentAnimation,previousAnimation,previousAnimationFramesLeft);
}

void Player::changeAnimation(PlayerAnimations newAnimation)
{
	if (currentAnimation != newAnimation)
	{
		previousAnimation = currentAnimation;
		currentAnimation = newAnimation;
		previousAnimationFramesLeft = 10;
	}
}

void Player::stopAttack()
{
	switch(actualAttack)
	{
	case AttackTypes::Short:
		attackingFramesLeft = 40;
		changeAnimation(PlayerAnimations::Cooldown);
		break;
	case AttackTypes::Long:
		attackingFramesLeft = 150;
		changeAnimation(PlayerAnimations::Cooldown);
		break;
	case AttackTypes::Super: break;
	default: ;
	}
	actualAttack = AttackTypes::NoAttack;
}
