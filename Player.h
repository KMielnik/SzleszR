#pragma once
#include "Entity.h"
#include <QVector3D>
#include <QQuaternion>
#include "MeshCollection.h"

class Player : public Entity
{
public:
	enum class AttackTypes
	{
		NoAttack,
		Short,
		Long,
		Super
	};
	~Player() override;
	Player(MeshCollection::ModelTexture modelTexture);
	void ShortAttack();
	void LongAttack();
	void SuperAttack();

	AttackTypes isAttacking();
	void PerformLogicStep() override;
	bool CheckCollision(Entity* entity) override;
	QVector3D GetColor();

	

	void Draw() override;
	
private:
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
