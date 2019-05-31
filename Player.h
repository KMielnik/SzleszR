#pragma once
#include "Entity.h"
#include <QVector3D>
#include <QQuaternion>
#include "MeshCollection.h"

class Player : public Entity
{
public:
	~Player() override;
	Player(MeshCollection::ModelTexture modelTexture);
	void Attack();

	bool isAttacking();
	void PerformLogicStep() override;
	bool CheckCollision(Entity* entity) override;
	QVector3D GetColor();
	
private:
	int HP = 100;
	float attackRadius = 1.5f;
	int attackingFramesLeft=0;
	QVector3D color;
};
