#include "AnimatedShader.h"
#include "Vertex.h"

void AnimatedShader::CompileShader()
{
	shaderProgram = new QOpenGLShaderProgram();
	shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexFile.c_str());
	shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentFile.c_str());

	shaderProgram->link();
	shaderProgram->bind();

	AnimatedShader::bindUniformLocations();
}

void AnimatedShader::SetVertexVBOData()
{
	//vertices
	shaderProgram->enableAttributeArray(0);
	shaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(AnimatedVertex));

	//normals
	shaderProgram->enableAttributeArray(1);
	shaderProgram->setAttributeBuffer(1, GL_FLOAT, sizeof(QVector3D), 3, sizeof(AnimatedVertex));

	//texture coordinates
	shaderProgram->enableAttributeArray(2);
	shaderProgram->setAttributeBuffer(2, GL_FLOAT, sizeof(QVector3D) * 2, 2, sizeof(AnimatedVertex));

	//cooldown coordinates
	shaderProgram->enableAttributeArray(3);
	shaderProgram->setAttributeBuffer(3, GL_FLOAT, sizeof(QVector3D) * 2 + sizeof(QVector2D), 3, sizeof(AnimatedVertex));

	//attack coordinates
	shaderProgram->enableAttributeArray(4);
	shaderProgram->setAttributeBuffer(4, GL_FLOAT, sizeof(QVector3D) * 3 + sizeof(QVector2D), 3, sizeof(AnimatedVertex));
}

#include <time.h> 
void AnimatedShader::SetAnimation(int animation)
{

	actualAnimation = time(NULL) % 2;
	shaderProgram->setUniformValue(actualAnimationLoc, actualAnimation);
}

void AnimatedShader::bindUniformLocations()
{
	Shader::bindUniformLocations();
	actualAnimationLoc = shaderProgram->uniformLocation("actualAnimation");

}