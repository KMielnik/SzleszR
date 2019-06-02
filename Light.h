#pragma once
#include <QVector3D>

#define MAX_LIGHTS 10

class Light
{
public:
	Light(QVector3D position, QVector3D color);
	~Light();
	void setPosition(QVector3D position);
	void setColor(QVector3D color);

	QVector3D position;
	QVector3D color;
};

