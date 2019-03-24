#pragma once
#include <QVector3D>

class Light
{
public:
	Light(QVector3D position, QVector3D color);
	~Light();

	QVector3D position;
	QVector3D color;
};

