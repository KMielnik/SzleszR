#include "Sphere.h"

Sphere::Sphere() : Entity(MeshCollection::ModelType::Sphere)
{

}

void Sphere::setColor(QVector3D color)
{
	this->color = color;
}

void Sphere::Draw()
{
	QMatrix4x4 transformations = initialTransformation;
	transformations.translate(position);
	transformations.rotate(rotation);

	meshCollection->Draw(modelType, color,
		transformations);
}
