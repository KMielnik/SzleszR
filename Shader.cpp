#include "Shader.h"
#include "Vertex.h"


Shader::Shader(QMatrix4x4* projectionMatrix, QMatrix4x4* cameraMatrix, std::vector<Light*> *lights)
{
	vertexFile = "Resources/Shaders/simpleModel.vert";
	fragmentFile = "Resources/Shaders/simpleTextured.frag";

	shaderProgram = new QOpenGLShaderProgram();
	shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexFile.c_str());
	shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentFile.c_str());

	this->projectionMatrix = projectionMatrix;
	this->cameraMatrix = cameraMatrix;
	this->lights = lights;

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
	LoadCameraMatrix(*cameraMatrix);
	LoadProjectionMatrix(*projectionMatrix);
	LoadLights(*lights);
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

void Shader::LoadLights(std::vector<Light *> light)
{
	QVector3D positions[MAX_LIGHTS];
	QVector3D colors[MAX_LIGHTS];

	for(int i=0;i<light.size();i++)
	{
		positions[i] = light[i]->position;
		colors[i] = light[i]->color;
	}
	
	shaderProgram->setUniformValueArray(lightsPositionLoc,positions ,MAX_LIGHTS);
	shaderProgram->setUniformValueArray(lightsColorLoc,colors, MAX_LIGHTS);

	shaderProgram->setUniformValue(lightsCountLoc, GLint(light.size()));
}

void Shader::LoadShineDamper(float shineDamper)
{
	shaderProgram->setUniformValue(shineDamperLoc, shineDamper);
}

void Shader::SetVertexVBOData()
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

void Shader::LoadProjectionMatrix(QMatrix4x4 projectionMatrix)
{
	shaderProgram->setUniformValue(projectionMatrixLoc, projectionMatrix);
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

void ColorShader::LoadColor(int r, int g, int b)
{
	shaderProgram->setUniformValue(colorLoc, QVector3D(r, g, b));
}

void ColorShader::LoadColor(QVector3D color)
{
	shaderProgram->setUniformValue(colorLoc, color);
}

void ColorShader::bindUniformLocations()
{
	colorLoc = shaderProgram->uniformLocation("color");
}
