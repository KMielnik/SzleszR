#pragma once
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QVector3D>
#include <QVector2D>
#include "Shader.h"
#include "Vertex.h"


class Mesh
{
public:
	void Draw();
	std::vector<Vertex> vertices;

	Mesh(std::vector<Vertex> vertices, Shader* shaderProgram);
	~Mesh();

private:
	QOpenGLVertexArrayObject VAO;
	QOpenGLVertexArrayObject::Binder *VAOBinder;
	QOpenGLBuffer *VBO;

	Shader* shaderProgram;
};
