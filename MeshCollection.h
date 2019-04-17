#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <QOpenGLShaderProgram>
#include "Mesh.h"
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include "Shader.h"

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
	enum class ModelType { Robot, Terrain };
	enum class ModelTexture { Robot_Basic, Robot_Red };

	void Initialize(Shader *shaderProgram);
	void InitializeModel(ModelType modelType);
	void InitializeTexture(ModelTexture modelTexture);
	void Draw(ModelType modelType, ModelTexture modelTexture, QMatrix4x4 transformations);
	~MeshCollection();
	static MeshCollection* GetInstance();

private:
	QOpenGLFunctions *glFunc;

	MeshCollection() = default;
	static MeshCollection* instance;
	Assimp::Importer importer;
	std::map< ModelType, Mesh*> meshes;
	std::map<ModelTexture, Texture*> textures;
	Shader* shaderProgram;

	void InitializeRawModel(std::string path, ModelType modelType);
	void InitializeRawTexture(std::string path, std::string fileExtension, ModelTexture modelTexture);
};

