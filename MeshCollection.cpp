#include "MeshCollection.h"
#include <QDebug>

void MeshCollection::Initialize(QOpenGLShaderProgram *shaderProgram)
{
	this->shaderProgram = shaderProgram;
	modelTransformationsLoc = shaderProgram->uniformLocation("modelTransformations");
}

void MeshCollection::InitializeModel(ModelType modelType)
{
	if (meshes.find(modelType) != meshes.end())
		return;

	switch (modelType)
	{
	case ModelType::Man:
		InitializeRawModel("Resources/man.fbx", modelType);
		qDebug() << "Initialized model: Man";
		break;
	default:
		qDebug() << "ERROR: Unknown modeType.";
		break;
	}
}

void MeshCollection::Draw(ModelType modelType, QMatrix4x4 transformations)
{
	shaderProgram->setUniformValue(modelTransformationsLoc, transformations);

	if (meshes.find(modelType) != meshes.end())
		meshes[modelType]->Draw();
	else
		qDebug() << "ERROR: Tried to use unitialized mesh.";
}


MeshCollection::~MeshCollection()
{
	for (auto mesh : meshes)
		delete mesh.second;
	meshes.clear();
}

MeshCollection* MeshCollection::instance = nullptr;

MeshCollection* MeshCollection::GetInstance()
{
	if (!instance)
		instance = new MeshCollection();
	return instance;
}

void MeshCollection::InitializeRawModel(std::string path, ModelType modelType)
{
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		qDebug() << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}

	aiMesh* mesh = scene->mMeshes[0];
	qDebug() << scene->mNumMaterials;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	//vertices
	for (auto i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.Position = QVector3D(
			mesh->mVertices[i].x/100 ,
			mesh->mVertices[i].y/100 ,
			mesh->mVertices[i].z/100
		);
		vertex.Normal = QVector3D(
			mesh->mNormals[i].x,
			mesh->mNormals[i].y-0.5,
			mesh->mNormals[i].z
		);

		vertex.TexCoords = QVector2D(
			mesh->mTextureCoords[0][i].x,
			mesh->mTextureCoords[0][i].y
		);
		vertices.push_back(vertex);
	}

	

	meshes[modelType] = new Mesh(vertices, textures, shaderProgram);
}
