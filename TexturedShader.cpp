#include "TexturedShader.h"
#include "Vertex.h"

void TexturedShader::CompileShader()
{
	shaderProgram = new QOpenGLShaderProgram();
	shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexFile.c_str());
	shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentFile.c_str());

	shaderProgram->link();
	shaderProgram->bind();

	TexturedShader::bindUniformLocations();
}

void TexturedShader::bindUniformLocations()
{
	Shader::bindUniformLocations();
}

void TexturedShader::SetVertexVBOData()
{
	//vertices
	shaderProgram->enableAttributeArray(0);
	shaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(Vertex));

	//normals
	shaderProgram->enableAttributeArray(1);
	shaderProgram->setAttributeBuffer(1, GL_FLOAT, sizeof(QVector3D), 3, sizeof(Vertex));

	//texture coordinates
	shaderProgram->enableAttributeArray(2);
	shaderProgram->setAttributeBuffer(2, GL_FLOAT, sizeof(QVector3D) * 2, 2, sizeof(Vertex));
}