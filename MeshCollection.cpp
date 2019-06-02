#include "MeshCollection.h"
#include <QDebug>


void MeshCollection::Initialize(QMatrix4x4* projectionMatrix, QMatrix4x4* cameraMatrix, std::vector<Light*>* lights)
{
	this->basicShaderProgram = new TexturedShader(projectionMatrix, cameraMatrix, lights);
	this->colorShaderProgram = new ColorShader(projectionMatrix, cameraMatrix, lights);
	this->animatedShaderProgram = new AnimatedShader(projectionMatrix, cameraMatrix, lights);

	basicShaderProgram->CompileShader();
	colorShaderProgram->CompileShader();
	animatedShaderProgram->CompileShader();

	glFunc = QOpenGLContext::currentContext()->functions();
}

void MeshCollection::InitializeModel(ModelType modelType)
{
	if (meshes.find(modelType) != meshes.end())
		return;

	switch (modelType)
	{
	case ModelType::Robot:
		InitializeRawModelWithAnimations("Resources/Models/robot", modelType);
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

	default:
		qDebug() << "ERROR: Unknown modelTexture.";
		break;
	}
}


void MeshCollection::Draw(ModelType modelType, ModelTexture modelTexture, QMatrix4x4 transformations)
{
	basicShaderProgram->Bind();
	basicShaderProgram->LoadModelTransformationsMatrix(transformations);

	if (textures.find(modelTexture) != textures.end())
	{
		glFunc->glActiveTexture(GL_TEXTURE0);
		textures[modelTexture]->diffuse->bind();
		glFunc->glActiveTexture(GL_TEXTURE1);
		textures[modelTexture]->specular->bind();
		basicShaderProgram->LoadShineDamper(textures[modelTexture]->shineDamper);

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

void MeshCollection::Draw(ModelType modelType, ModelTexture modelTexture, QMatrix4x4 transformations, PlayerAnimations animation, PlayerAnimations previousAnimation,int framesLeft)
{
	animatedShaderProgram->Bind();
	animatedShaderProgram->LoadModelTransformationsMatrix(transformations);


	if (textures.find(modelTexture) != textures.end())
	{
		glFunc->glActiveTexture(GL_TEXTURE0);
		textures[modelTexture]->diffuse->bind();
		glFunc->glActiveTexture(GL_TEXTURE1);
		textures[modelTexture]->specular->bind();

		animatedShaderProgram->LoadShineDamper(textures[modelTexture]->shineDamper);


	}
	else
		qDebug() << "ERROR: Tried to use unitialized texture.";

	if (meshes.find(modelType) != meshes.end())
		static_cast<PlayerMesh*>(meshes[modelType])->Draw(animation,previousAnimation,framesLeft);
	else
		qDebug() << "ERROR: Tried to use unitialized mesh.";

	textures[modelTexture]->specular->release();
	textures[modelTexture]->diffuse->release();
}

	

void MeshCollection::Draw(ModelType modelType, QVector3D color, QMatrix4x4 transformations)
{
	colorShaderProgram->Bind();
	colorShaderProgram->LoadModelTransformationsMatrix(transformations);

	colorShaderProgram->LoadColor(color);

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

	for (auto texture : textures)
		delete texture.second;
	textures.clear();

	delete basicShaderProgram;
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

	if (modelType == ModelType::Terrain)
		for (auto& vertex : vertices)
			vertex.TexCoords *= 15;;

	meshes[modelType] = new Mesh(vertices, basicShaderProgram);
}

void MeshCollection::InitializeRawModelWithAnimations(std::string path, ModelType modelType)
{
	const aiScene* scene = importer.ReadFile(path+".fbx", aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		qDebug() << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}

	std::vector<AnimatedVertex> aVertices;

	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[i];

		//vertices
		for (auto i = 0; i < mesh->mNumVertices; i++)
		{
			AnimatedVertex aVertex;
			aVertex.Normal = QVector3D(
				mesh->mNormals[i].x,
				mesh->mNormals[i].y,
				mesh->mNormals[i].z
			);

			aVertex.TexCoords = QVector2D(
				mesh->mTextureCoords[0][i].x,
				mesh->mTextureCoords[0][i].y
			);
			aVertices.push_back(aVertex);
		}
	}

	//default positions
	scene = importer.ReadFile(path + "_default.obj", aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		qDebug() << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}
	int x = 0;
	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[i];

		//vertices
		for (auto i = 0; i < mesh->mNumVertices; i++)
		{
			aVertices[x++].DefaultPosition = QVector3D(
				mesh->mVertices[i].x,
				mesh->mVertices[i].y,
				mesh->mVertices[i].z
			);

		}
	}

	//windup positions
	scene = importer.ReadFile(path + "_windup.obj", aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		qDebug() << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}
	x = 0;
	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[i];

		//vertices
		for (auto i = 0; i < mesh->mNumVertices; i++)
		{
			aVertices[x++].WindupPosition = QVector3D(
				mesh->mVertices[i].x,
				mesh->mVertices[i].y,
				mesh->mVertices[i].z
			);

		}
	}

	//attack short positions
	scene = importer.ReadFile(path + "_attack_short.obj", aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		qDebug() << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}
	 x = 0;
	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[i];

		//vertices
		for (auto i = 0; i < mesh->mNumVertices; i++)
		{
			aVertices[x++].AttackShortPosition = QVector3D(
				mesh->mVertices[i].x,
				mesh->mVertices[i].y,
				mesh->mVertices[i].z
			);

		}
	}

	//attack short positions
	scene = importer.ReadFile(path + "_attack_long.obj", aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		qDebug() << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}
	x = 0;
	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[i];

		//vertices
		for (auto i = 0; i < mesh->mNumVertices; i++)
		{
			aVertices[x++].AttackLongPosition = QVector3D(
				mesh->mVertices[i].x,
				mesh->mVertices[i].y,
				mesh->mVertices[i].z
			);

		}
	}

	//cooldown positions
	scene = importer.ReadFile(path + "_cooldown.obj", aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		qDebug() << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}
	x = 0;
	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[i];

		//vertices
		for (auto i = 0; i < mesh->mNumVertices; i++)
		{
			aVertices[x++].CooldownPosition = QVector3D(
				mesh->mVertices[i].x,
				mesh->mVertices[i].y,
				mesh->mVertices[i].z
			);

		}
	}

	meshes[modelType] = new PlayerMesh(aVertices, animatedShaderProgram);

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
