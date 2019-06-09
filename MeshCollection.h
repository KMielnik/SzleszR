#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <QOpenGLShaderProgram>
#include "Mesh.h"
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include "TexturedShader.h"
#include "ColorShader.h"

struct Texture
{
	QOpenGLTexture* diffuse;
	QOpenGLTexture* specular;
	float shineDamper;


	Texture(QOpenGLTexture* diffuse, QOpenGLTexture* specular, float shineDamper)
		: diffuse(diffuse), specular(specular), shineDamper(shineDamper) {}
	~Texture()
	{
		delete diffuse;
		delete specular;
	}
};

class MeshCollection
{
public:
	enum class ModelType { Robot, Terrain, Sphere, Rectangle };
	enum class ModelTexture { Robot_Basic, Robot_Red, Terrain, Sphere };

	void Initialize(QMatrix4x4* projectionMatrix, QMatrix4x4* cameraMatrix, std::vector<Light*>* lights);
	void InitializeModel(ModelType modelType);
	void InitializeTexture(ModelTexture modelTexture);
	void Draw(ModelType modelType, ModelTexture modelTexture, QMatrix4x4 transformations);
	void Draw(ModelType modelType, ModelTexture modelTexture, QMatrix4x4 transformations, PlayerAnimations animation, PlayerAnimations previousAnimation, int framesLeft);
	void Draw(ModelType modelType, QVector3D color, QMatrix4x4 transformations);
	~MeshCollection();
	static MeshCollection* GetInstance();

private:
	QOpenGLFunctions *glFunc;

	MeshCollection() = default;
	static MeshCollection* instance;
	Assimp::Importer importer;
	std::map< ModelType, Mesh*> meshes;
	std::map<ModelTexture, Texture*> textures;
	Shader* basicShaderProgram;
	ColorShader* colorShaderProgram;
	AnimatedShader* animatedShaderProgram;

	void InitializeRawModel(std::string path, ModelType modelType);
	void InitializeRawModelWithAnimations(std::string path, ModelType modelType);
	void InitializeRawTexture(std::string path, std::string fileExtension, ModelTexture modelTexture);
};

