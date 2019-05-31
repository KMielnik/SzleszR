#pragma once
#include <QOpenGLShaderProgram>
#include "Light.h"

#define MAX_LIGHTS 10

class Shader
{
public:
	Shader();
	~Shader();

	void Bind();
	void Release();

	void LoadProjectionMatrix(QMatrix4x4 projectionMatrix);
	void LoadCameraMatrix(QMatrix4x4 cameraMatrix);
	void LoadModelTransformationsMatrix(QMatrix4x4 transformationsMatrix);
	void LoadLights(std::vector<Light*> light);
	void LoadShineDamper(float shineDamper);

	void SetVertexVBOData();

private:
	QOpenGLShaderProgram* shaderProgram;
	std::string vertexFile = "Resources/Shaders/simple.vert";
	std::string fragmentFile = "Resources/Shaders/simple.frag";

	void bindUniformLocations();

	int projectionMatrixLoc = 0;
	int cameraMatrixLoc = 0;
	int modelTransformationsLoc = 0;
	int lightsPositionLoc = 0;
	int lightsColorLoc = 0;
	int lightsCountLoc = 0;
	int shineDamperLoc = 0;
};

