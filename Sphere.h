#pragma once
#include "MeshCollection.h"
#include "Entity.h"


class Sphere : public Entity
{
public:
	Sphere();
	void setColor(QVector3D color);
	void Draw() override;

private:
	QVector3D color;
};
