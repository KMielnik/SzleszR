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

struct Texture
{
	unsigned int id;
	std::string type;
};

class Mesh
{
public:
	void Draw();
	std::vector<Vertex> vertices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices,
		std::vector<Texture> textures, QOpenGLShaderProgram* m_program);

private:
	QOpenGLVertexArrayObject VAO;
	QOpenGLVertexArrayObject::Binder *VAOBinder;
	QOpenGLBuffer *VBO;

	QOpenGLShaderProgram* shaderProgram;
};
