#pragma once
#include <QVector3D>

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

