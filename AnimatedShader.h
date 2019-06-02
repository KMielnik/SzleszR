#pragma once
#include "Shader.h"

enum class PlayerAnimations
{
	Ready,
	Windup,
	Attacking_Short,
	Attacking_Long,
	Cooldown
};

class AnimatedShader : public Shader
{
public:

	AnimatedShader(QMatrix4x4* projectionMatrix, QMatrix4x4* cameraMatrix, std::vector<Light*>* lights) : Shader(projectionMatrix, cameraMatrix, lights)
	{
		vertexFile = "Resources/Shaders/animatedModel.vert";
		fragmentFile = "Resources/Shaders/simpleTextured.frag";
	}

	void CompileShader() override;
	void SetAnimation(PlayerAnimations animation, PlayerAnimations previousAnimation, int framesLeft);
	void bindUniformLocations() override;

	void SetVertexVBOData() override;
private:
	PlayerAnimations actualAnimation;
	int actualAnimationLoc = 0;
	int previousAnimationLoc=0;
	int framesLeftLoc=0;
};
