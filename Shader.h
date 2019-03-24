#pragma once
#include <QOpenGLShaderProgram>

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

	void SetVertexVBOData();

private:
	QOpenGLShaderProgram* shaderProgram;
	std::string vertexFile = "Resources/Shaders/simple.vert";
	std::string fragmentFile = "Resources/Shaders/simple.frag";

	void bindUniformLocations();

	int projectionMatrixLoc = 0;
	int cameraMatrixLoc = 0;
	int modelTransformationsLoc = 0;
};

