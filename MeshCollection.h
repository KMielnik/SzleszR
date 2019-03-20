#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <QOpenGLShaderProgram>
#include "Mesh.h"

class MeshCollection
{
public:
	enum class ModelType { Man };

	MeshCollection(QOpenGLShaderProgram *shaderProgram);
	void InitializeModel(ModelType modelType);
	void Draw(ModelType modelType);
	~MeshCollection();

private:
	Assimp::Importer importer;
	std::map< ModelType, Mesh*> meshes;
	QOpenGLShaderProgram* shaderProgram;

	void InitializeRawModel(std::string path, ModelType modelType);
};

