#include "Terrain.h"



Terrain::Terrain() : Entity(MeshCollection::ModelType::Terrain,
	MeshCollection::ModelTexture::Robot_Red)
{
	rotation = QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), -90);
}


Terrain::~Terrain()
{
}
