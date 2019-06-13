#include "Player.h"
#include <QDebug>
#include <corecrt_math_defines.h>

Player::~Player()
{
}

bool Player::IsAlive()
{
	return HP > 0;
}

void Player::Suicide()
{
	HP = 0;
}

Player::Player(int id, MeshCollection::ModelTexture modelTexture) : Entity(MeshCollection::ModelType::Robot,modelTexture), id(id)
{
	switch(modelTexture)
	{
	case MeshCollection::ModelTexture::Robot_Basic:
		color = QVector3D(1, 1, 1);
		break;
	case MeshCollection::ModelTexture::Robot_Red:
		color = QVector3D(1, 0, 0);
		break;
	case MeshCollection::ModelTexture::Robot_Orange: 
		color = QVector3D(1, 0.5, 0); 
		break;
	case MeshCollection::ModelTexture::Robot_Yellow:
		color = QVector3D(1, 1, 0); 
		break;
	case MeshCollection::ModelTexture::Robot_Green:
		color = QVector3D(0, 1, 0.2); 
		break;
	case MeshCollection::ModelTexture::Robot_Blue:
		color = QVector3D(1, 0.3, 1); 
		break;
	case MeshCollection::ModelTexture::Robot_Purple:
		color = QVector3D(1, 0, 1); 
		break;
	default:
		color = QVector3D(1, 1, 1);
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

void Player::Block()
{
	if (attackingFramesLeft <= 0)
	{
		attackingFramesLeft = 100;
		actualAttack = AttackTypes::Block;
		changeAnimation(PlayerAnimations::Cooldown);
	}
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
	case AttackTypes::Block:
		return actualAttack;
		break;
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

bool pointInTriangle(QPointF pt, QPointF v1, QPointF v2, QPointF v3)
{
	auto sign = [](QPointF p1, QPointF p2, QPointF p3) { return (p1.x() - p3.x()) * (p2.y() - p3.y()) - (p2.x() - p3.x()) * (p1.y() - p3.y()); };
	float d1, d2, d3;
	bool has_neg, has_pos;

	d1 = sign(pt, v1, v2);
	d2 = sign(pt, v2, v3);
	d3 = sign(pt, v3, v1);

	has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
	has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

	return !(has_neg && has_pos);
}

bool TriangleAttack(Player* enemy, Player* player, float range, float angle)
{
	float horizontalDistance = range * std::cos(M_PI / 180);
	float theta = enemy->GetRotation().toEulerAngles().y() + angle;
	float offsetX = horizontalDistance * std::sin(theta * M_PI / 180);
	float offsetZ = horizontalDistance * std::cos(theta * M_PI / 180);

	QPointF rightPoint(enemy->GetPosition().x() + offsetX, enemy->GetPosition().z() + offsetZ);

	theta = enemy->GetRotation().toEulerAngles().y() - angle;
	offsetX = horizontalDistance * std::sin(theta * M_PI / 180);
	offsetZ = horizontalDistance * std::cos(theta * M_PI / 180);

	QPointF leftPoint(enemy->GetPosition().x() + offsetX, enemy->GetPosition().z() + offsetZ);

	QPointF enemyRoot(enemy->GetPosition().x(), enemy->GetPosition().z());

	QPointF playerRoot(player->GetPosition().x(), player->GetPosition().z());
	return pointInTriangle(playerRoot, enemyRoot, leftPoint, rightPoint);
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
			if (TriangleAttack(enemy, this, 4, 45))
			{
				if (actualAttack == AttackTypes::Block)
				{
					HP -= 2;
					qDebug() << "Player " << enemy->GetID() << " HIT BLOCKING" << GetID() << " FOR: " << 2;
				}
				else
				{
					HP -= 7;
					qDebug() << "Player " << enemy->GetID() << " HIT" << GetID() << " FOR: " << 7;
				}
				enemy->stopAttack();
				attackCollisionOccured = true;
			}
		}
		if (enemy->isAttacking() == AttackTypes::Long)
		{
			if (TriangleAttack(enemy, this, 6, 45))
			{
				if (actualAttack == AttackTypes::Block)
				{
					HP -= 8;
					qDebug() << "Player " << enemy->GetID() << " HIT BLOCKING" << GetID() << " FOR: " << 8;
				}
				else
				{
					HP -= 20;
					qDebug() << "Player " << enemy->GetID() << " HIT" << GetID() << " FOR: " << 20;
				}
				enemy->stopAttack();
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

void Player::Serialize(SerializedPlayer* buffer) const
{
	buffer->id = id;
	buffer->actualAttack = static_cast<int>(actualAttack);
	buffer->HP = HP;
	buffer->attackingFramesLeft = attackingFramesLeft;
	buffer->color = color;
	buffer->currentAnimation = currentAnimation;
	buffer->force = force;
	buffer->position = position;
	buffer->previousAnimation = previousAnimation;
	buffer->previousAnimationFramesLeft = previousAnimationFramesLeft;
	buffer->rotation = rotation;
	buffer->simpleMovement = simpleMovement;
}

void Player::Deserialize(SerializedPlayer buffer)
{
	id = buffer.id;
	actualAttack = static_cast<AttackTypes>(buffer.actualAttack);
	HP = buffer.HP;
	attackingFramesLeft = buffer.attackingFramesLeft;
	color = buffer.color;
	currentAnimation = buffer.currentAnimation;
	force = buffer.force;
	position = buffer.position;
	previousAnimation = buffer.previousAnimation;
	previousAnimationFramesLeft = buffer.previousAnimationFramesLeft;
	rotation = buffer.rotation;
	simpleMovement = buffer.simpleMovement;
}

int Player::GetID() const
{
	return id;
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
		attackingFramesLeft = 80;
		changeAnimation(PlayerAnimations::Cooldown);
		break;
	case AttackTypes::Block: break;
	default: ;
	}
	actualAttack = AttackTypes::NoAttack;
}
