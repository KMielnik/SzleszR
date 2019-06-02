#pragma once
#include "AnimatedShader.h"
#include "Mesh.h"

class PlayerMesh : public Mesh
{
public:
	PlayerMesh(std::vector<AnimatedVertex> aVertices, Shader* shaderProgram);
	void Draw(PlayerAnimations animation) override;
private:
	std::vector<AnimatedVertex> vertices;
	AnimatedShader* shaderProgram;

	void setMeshVAOVBO() override;
};
