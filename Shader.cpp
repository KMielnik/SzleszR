#include "Shader.h"
#include "Vertex.h"


Shader::Shader()
{
	shaderProgram = new QOpenGLShaderProgram();
	shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexFile.c_str());
	shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentFile.c_str());

	shaderProgram->link();
	shaderProgram->bind();

	bindUniformLocations();
}


Shader::~Shader()
{
}

void Shader::Bind()
{
	shaderProgram->bind();
}

void Shader::Release()
{
	shaderProgram->release();
}

void Shader::LoadCameraMatrix(QMatrix4x4 cameraMatrix)
{
	shaderProgram->setUniformValue(cameraMatrixLoc, cameraMatrix);
}

void Shader::LoadModelTransformationsMatrix(QMatrix4x4 transformationsMatrix)
{
	shaderProgram->setUniformValue(modelTransformationsLoc, transformationsMatrix);
}

void Shader::SetVertexVBOData()
{
	shaderProgram->enableAttributeArray(0);
	shaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(Vertex));

	//normals
	shaderProgram->enableAttributeArray(1);
	shaderProgram->setAttributeBuffer(1, GL_FLOAT, sizeof(QVector3D), 3, sizeof(Vertex));

	//texture coordinates
	shaderProgram->enableAttributeArray(2);
	shaderProgram->setAttributeBuffer(2, GL_FLOAT, sizeof(QVector3D) * 2, 2, sizeof(Vertex));
}

void Shader::LoadProjectionMatrix(QMatrix4x4 projectionMatrix)
{
	shaderProgram->setUniformValue(projectionMatrixLoc, projectionMatrix);
}

void Shader::bindUniformLocations()
{
	projectionMatrixLoc = shaderProgram->uniformLocation("projectionMatrix");
	cameraMatrixLoc = shaderProgram->uniformLocation("cameraMatrix");
	modelTransformationsLoc = shaderProgram->uniformLocation("modelTransformations");
}
