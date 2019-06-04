#pragma once
#include <QVector3D>
#include "MeshCollection.h"

#pragma pack(push, 1)
struct SerializedPlayer
{
	int id;
	QVector3D position;
	QVector2D simpleMovement;
	QVector3D force;
	QQuaternion rotation;
	MeshCollection::ModelTexture texture;
	int HP;
	int actualAttack;
	PlayerAnimations currentAnimation;
	PlayerAnimations previousAnimation;
	int previousAnimationFramesLeft;
	int attackingFramesLeft;
	QVector3D color;
	int magic = 777;

};
#pragma pack(pop)