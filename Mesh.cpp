#include "Mesh.h"
#include <QDebug>

void Mesh::Draw()
{
	VAOBinder->rebind();
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

Mesh::Mesh(std::vector<Vertex> vertices,
	std::vector<Texture> textures, QOpenGLShaderProgram* m_program)
{
	this->vertices = vertices;
	this->textures = textures;

	VAO.create();
	VAO.bind();
	VAOBinder = new QOpenGLVertexArrayObject::Binder(&VAO);

	VBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	VBO->create();
	VBO->bind();
	VBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
	VBO->allocate(vertices.data(), vertices.size() * sizeof(Vertex));

	//Vertices
	m_program->enableAttributeArray(0);
	m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(Vertex));

	//normals
	m_program->enableAttributeArray(1);
	m_program->setAttributeBuffer(1, GL_FLOAT, sizeof(QVector3D), 3, sizeof(Vertex));

	VBO->release();
	VAOBinder->release();
	VAO.release();
	m_program->release();
}

Mesh::~Mesh()
{
	VBO->destroy();
	delete VBO;
	delete VAOBinder;
}
