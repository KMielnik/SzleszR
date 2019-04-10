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

	
private:
	
};
