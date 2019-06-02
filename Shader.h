#pragma once
#include <QOpenGLShaderProgram>
#include "Light.h"
#include "Vertex.h"


class Shader
{
public:
	Shader(QMatrix4x4* projectionMatrix,QMatrix4x4* cameraMatrix , std::vector<Light*> *lights);
	virtual ~Shader();

	virtual void CompileShader();
	virtual void Bind();
	virtual void Release();

	virtual void LoadProjectionMatrix();
	virtual void LoadCameraMatrix();
	virtual void LoadLights();
	virtual void LoadShineDamper(float shineDamper);
	virtual void LoadModelTransformationsMatrix(QMatrix4x4 transformationsMatrix);
	virtual void SetVertexVBOData() = 0;

protected:
	QOpenGLShaderProgram* shaderProgram = nullptr;
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