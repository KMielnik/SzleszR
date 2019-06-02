﻿#include "Mesh.h"
#include <QDebug>

void Mesh::Draw(PlayerAnimations cheat)
{
	VAOBinder->rebind();
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

Mesh::Mesh(std::vector<Vertex> vertices, Shader* shaderProgram)
{
	this->vertices = vertices;
	this->shaderProgram = shaderProgram;

	Mesh::setMeshVAOVBO();
}

Mesh::~Mesh()
{
	VBO->destroy();
	delete VBO;
	delete VAOBinder;
}

void Mesh::setMeshVAOVBO()
{
	VAO.create();
	VAO.bind();
	VAOBinder = new QOpenGLVertexArrayObject::Binder(&VAO);

	VBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	VBO->create();
	VBO->bind();
	VBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
	VBO->allocate(vertices.data(), vertices.size() * sizeof(AnimatedVertex));

	shaderProgram->SetVertexVBOData();

	VBO->release();
	VAOBinder->release();
	VAO.release();

	shaderProgram->Release();
}
