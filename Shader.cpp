#include "Shader.h"

Shader::Shader(QMatrix4x4* projectionMatrix, QMatrix4x4* cameraMatrix, std::vector<Light*> *lights)
{
	this->projectionMatrix = projectionMatrix;
	this->cameraMatrix = cameraMatrix;
	this->lights = lights;
}

Shader::~Shader()
{
	delete shaderProgram;
}

void Shader::CompileShader()
{
	shaderProgram = new QOpenGLShaderProgram();
	shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexFile.c_str());
	shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentFile.c_str());

	shaderProgram->link();
	shaderProgram->bind();

	Shader::bindUniformLocations();
}

void Shader::Bind()
{
	shaderProgram->bind();

	LoadCameraMatrix();
	LoadProjectionMatrix();
	LoadLights();
}

void Shader::Release()
{
	shaderProgram->release();
}

void Shader::LoadCameraMatrix()
{
	shaderProgram->setUniformValue(cameraMatrixLoc, *cameraMatrix);
}

void Shader::LoadModelTransformationsMatrix(QMatrix4x4 transformationsMatrix)
{
	shaderProgram->setUniformValue(modelTransformationsLoc, transformationsMatrix);
}

void Shader::LoadLights()
{
	QVector3D positions[MAX_LIGHTS];
	QVector3D colors[MAX_LIGHTS];

	for(auto i=0;i<lights->size();i++)
	{
		positions[i] = (*lights)[i]->position;
		colors[i] = (*lights)[i]->color;
	}
	
	shaderProgram->setUniformValueArray(lightsPositionLoc,positions ,MAX_LIGHTS);
	shaderProgram->setUniformValueArray(lightsColorLoc,colors, MAX_LIGHTS);

	shaderProgram->setUniformValue(lightsCountLoc, GLint((*lights).size()));
}

void Shader::LoadShineDamper(float shineDamper)
{
	shaderProgram->setUniformValue(shineDamperLoc, shineDamper);
}

void Shader::LoadProjectionMatrix()
{
	shaderProgram->setUniformValue(projectionMatrixLoc, *projectionMatrix);
}

void Shader::bindUniformLocations()
{
	projectionMatrixLoc = shaderProgram->uniformLocation("projectionMatrix");
	cameraMatrixLoc = shaderProgram->uniformLocation("cameraMatrix");
	modelTransformationsLoc = shaderProgram->uniformLocation("modelTransformations");
	lightsPositionLoc = shaderProgram->uniformLocation("lightsPosition");
	lightsColorLoc = shaderProgram->uniformLocation("lightsColors");
	lightsCountLoc = shaderProgram->uniformLocation("lightsCount");
	shineDamperLoc = shaderProgram->uniformLocation("shineDamper");
}
