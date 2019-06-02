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
	//ready stance position
	shaderProgram->enableAttributeArray(0);
	shaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(AnimatedVertex));

	//normals
	shaderProgram->enableAttributeArray(1);
	shaderProgram->setAttributeBuffer(1, GL_FLOAT, sizeof(QVector3D), 3, sizeof(AnimatedVertex));

	//texture coordinates
	shaderProgram->enableAttributeArray(2);
	shaderProgram->setAttributeBuffer(2, GL_FLOAT, sizeof(QVector3D) * 2, 2, sizeof(AnimatedVertex));

	//windup stance position
	shaderProgram->enableAttributeArray(3);
	shaderProgram->setAttributeBuffer(3, GL_FLOAT, sizeof(QVector3D) * 2 + sizeof(QVector2D), 3, sizeof(AnimatedVertex));

	//Attacking stance position
	shaderProgram->enableAttributeArray(4);
	shaderProgram->setAttributeBuffer(4, GL_FLOAT, sizeof(QVector3D) * 3 + sizeof(QVector2D), 3, sizeof(AnimatedVertex));

	//Cooldown stance position
	shaderProgram->enableAttributeArray(5);
	shaderProgram->setAttributeBuffer(5, GL_FLOAT, sizeof(QVector3D) * 4 + sizeof(QVector2D), 3, sizeof(AnimatedVertex));
}

void AnimatedShader::SetAnimation(PlayerAnimations animation)
{
	actualAnimation = animation;

	int animationInShader;

	switch (actualAnimation)
	{
	case PlayerAnimations::Ready: animationInShader = 0; break;
	case PlayerAnimations::Windup: animationInShader = 1;  break;
	case PlayerAnimations::Attacking: animationInShader = 2;  break;
	case PlayerAnimations::Cooldown: animationInShader = 3;  break;
	default:  animationInShader = 0; ;
	}

	shaderProgram->setUniformValue(actualAnimationLoc, animationInShader);
}

void AnimatedShader::bindUniformLocations()
{
	Shader::bindUniformLocations();
	actualAnimationLoc = shaderProgram->uniformLocation("actualAnimation");
}