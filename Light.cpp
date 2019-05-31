#include "Light.h"



Light::Light(QVector3D position, QVector3D color) :position(position), color(color)
{
}


Light::~Light()
{
}

void Light::setPosition(QVector3D position)
{
	this->position = position;
}

void Light::setColor(QVector3D color)
{
	this->color = color;
}
