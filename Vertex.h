#pragma once
#include <QVector2D>
#include <QVector3D>

struct Vertex
{
	QVector3D Position;
	QVector3D Normal;
	QVector2D TexCoords;
};

struct AnimatedVertex
{
	QVector3D DefaultPosition;
	QVector3D Normal;
	QVector2D TexCoords;
	QVector3D WindupPosition;
	QVector3D AttackPosition;
	QVector3D CooldownPosition;
};