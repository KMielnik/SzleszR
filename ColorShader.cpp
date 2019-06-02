#include "ColorShader.h"
#include "Vertex.h"

void ColorShader::CompileShader()
{
	shaderProgram = new QOpenGLShaderProgram();
	shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexFile.c_str());
	shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentFile.c_str());

	shaderProgram->link();
	shaderProgram->bind();

	ColorShader::bindUniformLocations();
}

void ColorShader::LoadColor(int r, int g, int b) const
{
	shaderProgram->setUniformValue(colorLoc, QVector3D(r, g, b));
}

void ColorShader::LoadColor(QVector3D color) const
{
	shaderProgram->setUniformValue(colorLoc, color);
}

void ColorShader::bindUniformLocations()
{
	Shader::bindUniformLocations();
	colorLoc = shaderProgram->uniformLocation("color");
}

void ColorShader::SetVertexVBOData()
{
	//vertices
	shaderProgram->enableAttributeArray(0);
	shaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(Vertex));

	//normals
	shaderProgram->enableAttributeArray(1);
	shaderProgram->setAttributeBuffer(1, GL_FLOAT, sizeof(QVector3D), 3, sizeof(Vertex));
}