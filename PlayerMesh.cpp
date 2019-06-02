#include "PlayerMesh.h"

PlayerMesh::PlayerMesh(std::vector<AnimatedVertex> vertices, Shader* shaderProgram)
{
	this->vertices = vertices;
	this->shaderProgram = dynamic_cast<AnimatedShader*>(shaderProgram);

	PlayerMesh::setMeshVAOVBO();
}

void PlayerMesh::Draw(PlayerAnimations animation)
{
	shaderProgram->SetAnimation(animation);

	VAOBinder->rebind();
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

void PlayerMesh::setMeshVAOVBO()
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