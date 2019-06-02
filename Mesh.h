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
	virtual void Draw();
	
	Mesh(std::vector<Vertex> vertices, Shader* shaderProgram);
	~Mesh();

protected:
	QOpenGLVertexArrayObject VAO;
	QOpenGLVertexArrayObject::Binder *VAOBinder;
	QOpenGLBuffer *VBO;
	Mesh() = default;

private:
	std::vector<Vertex> vertices;
};

class PlayerMesh : public Mesh
{
public:
	PlayerMesh(std::vector<AnimatedVertex> aVertices, Shader* shaderProgram);
	void Draw() override;
private:
	std::vector<AnimatedVertex> vertices;
	AnimatedShader* shader;
};
