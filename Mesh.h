#pragma once
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QVector3D>
#include <QVector2D>
#include "Shader.h"
#include "Vertex.h"
#include "AnimatedShader.h"


class Mesh
{
public:
	virtual void Draw(PlayerAnimations cheat = PlayerAnimations::Ready);
	
	Mesh(std::vector<Vertex> vertices, Shader* shaderProgram);
	virtual ~Mesh();

protected:
	QOpenGLVertexArrayObject VAO;
	QOpenGLVertexArrayObject::Binder *VAOBinder;
	QOpenGLBuffer *VBO;
	Mesh() = default;
	virtual void setMeshVAOVBO();

private:
	std::vector<Vertex> vertices;
	Shader* shaderProgram;
};

