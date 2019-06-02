#pragma once
#include <QOpenGLShaderProgram>
#include "Light.h"

#define MAX_LIGHTS 10

class Shader
{
public:
	Shader(QMatrix4x4* projectionMatrix,QMatrix4x4* cameraMatrix , std::vector<Light*> *lights);
	~Shader();

	void Bind();
	void Release();

	void LoadProjectionMatrix(QMatrix4x4 projectionMatrix);
	void LoadCameraMatrix(QMatrix4x4 cameraMatrix);
	void LoadModelTransformationsMatrix(QMatrix4x4 transformationsMatrix);
	void LoadLights(std::vector<Light*> light);
	void LoadShineDamper(float shineDamper);

	virtual void SetVertexVBOData();

protected:
	QOpenGLShaderProgram* shaderProgram;
	std::string vertexFile;
	std::string fragmentFile;

	QMatrix4x4* projectionMatrix;
	QMatrix4x4* cameraMatrix;
	std::vector<Light*> *lights;

	virtual void bindUniformLocations();

	int projectionMatrixLoc = 0;
	int cameraMatrixLoc = 0;
	int modelTransformationsLoc = 0;
	int lightsPositionLoc = 0;
	int lightsColorLoc = 0;
	int lightsCountLoc = 0;
	int shineDamperLoc = 0;
};

class ColorShader : public Shader
{
public:
	ColorShader(QMatrix4x4* projectionMatrix, QMatrix4x4* cameraMatrix, std::vector<Light*>* lights) : Shader(projectionMatrix,cameraMatrix,lights)
	{
		vertexFile = "Resources/Shaders/simpleModel.vert";
		fragmentFile = "Resources/Shaders/simpleColored.frag";

		shaderProgram = new QOpenGLShaderProgram();
		shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexFile.c_str());
		shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentFile.c_str());

		this->projectionMatrix = projectionMatrix;
		this->cameraMatrix = cameraMatrix;
		this->lights = lights;

		shaderProgram->link();
		shaderProgram->bind();

		ColorShader::bindUniformLocations();
	}

	void LoadColor(int r, int g, int b);
	void LoadColor(QVector3D color);
	void bindUniformLocations() override;

private:
	int colorLoc = 0;
};

class AnimatedShader : public Shader
{
public:
	AnimatedShader(QMatrix4x4* projectionMatrix, QMatrix4x4* cameraMatrix, std::vector<Light*>* lights) : Shader(projectionMatrix, cameraMatrix, lights)
	{
		vertexFile = "Resources/Shaders/animatedModel.vert";
		fragmentFile = "Resources/Shaders/simpleTextured.frag";

		shaderProgram = new QOpenGLShaderProgram();
		shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexFile.c_str());
		shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentFile.c_str());

		this->projectionMatrix = projectionMatrix;
		this->cameraMatrix = cameraMatrix;
		this->lights = lights;

		shaderProgram->link();
		shaderProgram->bind();

		AnimatedShader::bindUniformLocations();
	}

	void SetVertexVBOData() override;
	void SetAnimation(int animation);
	void bindUniformLocations() override;

private:
	int actualAnimation;
	int actualAnimationLoc=0;
};
