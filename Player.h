﻿#pragma once
#include "Entity.h"
#include <QVector3D>
#include <QQuaternion>
#include "MeshCollection.h"
#include "SerializedPlayer.h"

class Player : public Entity
{
public:
	enum class AttackTypes
	{
		NoAttack,
		Short,
		Long,
		Block
	};
	~Player() override;
	bool IsAlive();
	void Suicide();
	Player(int id, MeshCollection::ModelTexture modelTexture);
	void ShortAttack();
	void LongAttack();
	void Block();

	AttackTypes isAttacking();
	void PerformLogicStep() override;
	bool CheckCollision(Entity* entity) override;
	QVector3D GetColor();

	void Serialize(SerializedPlayer* buffer) const;
	void Deserialize(SerializedPlayer buffer);

	int GetID() const;

	void Draw() override;
	
private:
	int id;
	int HP = 100;
	float attackRadius = 1.5f;

	float shortAttackFrames = 70;
	float longAttackFrames = 170;
	AttackTypes actualAttack;

	void changeAnimation(PlayerAnimations newAnimation);
	PlayerAnimations currentAnimation;
	PlayerAnimations previousAnimation;
	int previousAnimationFramesLeft=0;
	int attackingFramesLeft=0;
	QVector3D color;

	void stopAttack();
};
