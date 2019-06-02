#include "Mesh.h"
#include <QDebug>

void Mesh::Draw()
{
	VAOBinder->rebind();
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

Mesh::Mesh(std::vector<Vertex> vertices, Shader* shaderProgram)
{
	this->vertices = vertices;

	VAO.create();
	VAO.bind();
	VAOBinder = new QOpenGLVertexArrayObject::Binder(&VAO);

	VBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	VBO->create();
	VBO->bind();
	VBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
	VBO->allocate(vertices.data(), vertices.size() * sizeof(Vertex));

	shaderProgram->SetVertexVBOData();

	VBO->release();
	VAOBinder->release();
	VAO.release();

	shaderProgram->Release();
}

Mesh::~Mesh()
{
	VBO->destroy();
	delete VBO;
	delete VAOBinder;
}

PlayerMesh::PlayerMesh(std::vector<AnimatedVertex> vertices, Shader* shaderProgram)
{
	this->vertices = vertices;

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

	this->shader = static_cast<AnimatedShader*>(shaderProgram);
}

void PlayerMesh::Draw(PlayerAnimations animation, PlayerAnimations previousAnimation, int framesLeft)
{
	shader->SetAnimation(animation,previousAnimation,framesLeft);
	VAOBinder->rebind();
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}
