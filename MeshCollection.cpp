#include "MeshCollection.h"
#include <QDebug>

void MeshCollection::Initialize(Shader *shaderProgram)
{
	this->shaderProgram = shaderProgram;
	glFunc = QOpenGLContext::currentContext()->functions();
}

void MeshCollection::InitializeModel(ModelType modelType)
{
	if (meshes.find(modelType) != meshes.end())
		return;

	switch (modelType)
	{
	case ModelType::Robot:
		InitializeRawModel("Resources/Models/robot.fbx", modelType);
		qDebug() << "Initialized model: Robot";
		break;
	case ModelType::Terrain:
		InitializeRawModel("Resources/Models/terrain.fbx", modelType);
		qDebug() << "Initialized model: Terrain";
		break;
	default:
		qDebug() << "ERROR: Unknown modelType.";
		break;
	case ModelType::Sphere:
		InitializeRawModel("Resources/Models/sphere.fbx", modelType);
		qDebug() << "Initialized model: Sphere";
		break;
	}
}

void MeshCollection::InitializeTexture(ModelTexture modelTexture)
{
	if (textures.find(modelTexture) != textures.end())
		return;

	switch (modelTexture)
	{
	case ModelTexture::Robot_Basic:
		InitializeRawTexture("Resources/textures/robot_basic_",".png", modelTexture);
		break;
	
	case ModelTexture::Robot_Red: 
		InitializeRawTexture("Resources/textures/robot_red_", ".png", modelTexture);
		break;

	case ModelTexture::Terrain:
		InitializeRawTexture("Resources/textures/terrain_", ".png", modelTexture);
		break;

	case ModelTexture::Sphere:
		InitializeRawTexture("Resources/textures/sphere_", ".png", modelTexture);
		break;

	default:
		qDebug() << "ERROR: Unknown modelTexture.";
		break;
	}
}


void MeshCollection::Draw(ModelType modelType, ModelTexture modelTexture, QMatrix4x4 transformations)
{
	shaderProgram->Bind();
	shaderProgram->LoadModelTransformationsMatrix(transformations);

	if (textures.find(modelTexture) != textures.end())
	{
		glFunc->glActiveTexture(GL_TEXTURE0);
		textures[modelTexture]->diffuse->bind();
		glFunc->glActiveTexture(GL_TEXTURE1);
		textures[modelTexture]->specular->bind();
		shaderProgram->LoadShineDamper(textures[modelTexture]->shineDamper);
	}
	else
		qDebug() << "ERROR: Tried to use unitialized texture.";

	if (meshes.find(modelType) != meshes.end())
		meshes[modelType]->Draw();
	else
		qDebug() << "ERROR: Tried to use unitialized mesh.";

	textures[modelTexture]->specular->release();
	textures[modelTexture]->diffuse->release();
}


MeshCollection::~MeshCollection()
{
	for (auto mesh : meshes)
		delete mesh.second;
	meshes.clear();

	for (auto texture : textures)
		delete texture.second;
	textures.clear();
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

	std::vector<Vertex> vertices;

	for (int i =0; i < scene->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[i];

		//vertices
		for (auto i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			vertex.Position = QVector3D(
				mesh->mVertices[i].x,
				mesh->mVertices[i].y,
				mesh->mVertices[i].z 
			);
			vertex.Normal = QVector3D(
				mesh->mNormals[i].x,
				mesh->mNormals[i].y,
				mesh->mNormals[i].z
			);

			vertex.TexCoords = QVector2D(
				mesh->mTextureCoords[0][i].x,
				mesh->mTextureCoords[0][i].y
			);
			vertices.push_back(vertex);
		}
	}

	meshes[modelType] = new Mesh(vertices, shaderProgram);
}

void MeshCollection::InitializeRawTexture(std::string path, std::string fileExtension, ModelTexture modelTexture)
{
	auto diffuse = new QOpenGLTexture(QImage((path + "diffuse" + fileExtension).c_str()));

	diffuse->setAutoMipMapGenerationEnabled(true);
	diffuse->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
	diffuse->setMagnificationFilter(QOpenGLTexture::Linear);

	auto specular = new QOpenGLTexture(QImage((path + "specular" + fileExtension).c_str()));

	specular->setAutoMipMapGenerationEnabled(true);
	specular->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
	specular->setMagnificationFilter(QOpenGLTexture::Linear);

	auto texture = new Texture(diffuse, specular, 10);
	textures[modelTexture] = texture;
}
