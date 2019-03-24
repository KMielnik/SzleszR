#pragma once
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QVector3D>
#include <QVector2D>


struct Vertex
{
	QVector3D Position;
	QVector3D Normal;
	QVector2D TexCoords;
};

class Mesh
{
public:
	void Draw();
	std::vector<Vertex> vertices;

	Mesh(std::vector<Vertex> vertices, QOpenGLShaderProgram* m_program);
	~Mesh();

private:
	QOpenGLVertexArrayObject VAO;
	QOpenGLVertexArrayObject::Binder *VAOBinder;
	QOpenGLBuffer *VBO;

	QOpenGLShaderProgram* shaderProgram;
};
