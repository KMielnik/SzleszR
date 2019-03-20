#include "MeshCollection.h"
#include <QDebug>

MeshCollection::MeshCollection(QOpenGLShaderProgram *shaderProgram)
{
	this->shaderProgram = shaderProgram;
}

void MeshCollection::InitializeModel(ModelType modelType)
{
	switch (modelType)
	{
	case ModelType::BUNNY:
		InitializeRawModel("Resources/man.fbx", modelType);
		qDebug() << "Initialized model: BUNNY";
		break;
	default:
		qDebug() << "ERROR: Unknown modeType.";
		break;
	}
}

void MeshCollection::Draw(ModelType modelType)
{
	if (meshes.find(modelType) != meshes.end())
		meshes[modelType]->Draw();
	else
		qDebug() << "ERROR: Tried to use unitialized mesh.";
}


MeshCollection::~MeshCollection()
{
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

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	//vertices
	for (auto i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.Position = QVector3D(
			mesh->mVertices[i].x/100 +0.5,
			mesh->mVertices[i].y/100 -0.3,
			mesh->mVertices[i].z/100
		);
		vertex.Normal = QVector3D(
			mesh->mNormals[i].x,
			mesh->mNormals[i].y,
			mesh->mNormals[i].z
		);
		if (0)
			vertex.TexCoords = QVector2D(
				mesh->mTextureCoords[i]->x,
				mesh->mTextureCoords[i]->y
			);
		vertices.push_back(vertex);
	}

	//TODO textures importing

	meshes[modelType] = new Mesh(vertices, textures, shaderProgram);
}
